#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//network specifications
#include "settings.h"
#include "neuron.h"

//generate neurons and connections
#include "generate.h"
//draw
#include "drawShapes.h"

//statistics functions
#include "statistics.h"
//main update function
#include "pulse2.h"

typedef struct _threadArgs{
    int len;
    int* inputs;
} threadArgs;

//currently everything on one thread
void *thread(void *vargp) {
    // initialize arrays
    for(int i=0;i<OUT_NUM;i++) {
        correctOutputs[i]=0;
        tempOutputs[i]=0;
        firedOutputs[i]=0;
    }
    for(int i=0;i<REC_NUM;i++) {
        previousFired[i]=0;
        currentFired[i]=0;
        tempRecurrent[i]=0;
    }
    for(int i=0;i<trainedNeuronsNum;i++)
        correctOutputs[trainedNeurons[i]]=1;

    fireInputs();
    sendPulse();

    return NULL;
}

//terminal status print
void printStatus() {
    printf("Status\n");
    printf("\n");
    printf("seed: %d\n", seed);
    printf("tick: %d\n", tickCount);

    printf("avgActivitiy: %f\n", avgAct);

    for(int i=0;i<OUT_NUM;i++)
        printf("Output %d activity: %f\n", i, outputAvg[i]);
    // --
    printf("Output 1 potential: %f\n", outputs[0]->potential);
    printf("Output 2 potential: %f", outputs[1]->potential);

    printf("\n");
    printf("Dopamine: %f\n", dopamine);
    printf("Decay: %f\n", decay);

    printf("\n");
    printf("Params\n");
    printf("threshold: %f\n", threshold);
    printf("dpeak: %f\n", dpeak);
    printf("reward: %d\n", reward);
    printf("fatigue: %f\n", fatigue);
    printf("\n");
    printf("restore: %d\n", restore);
    //printf("trainNum: %d\n", trainNum);

    printf("\n");

    //move cursor up 18 + OUT_NUM lines
    for(int i=0;i<18+OUT_NUM;i++)
        printf("\r\033[F");

    fflush(stdout);
}

int main() {
    srand(seed);

    //GLFW setup ------------------------
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    if(visualize) {
        window = glfwCreateWindow(1200, 700, "Nervous", NULL, NULL);
        if (!window) {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);
    }

    int width, height;
    double cx, cy;
    //------------------------------------

    //create nerves
    createNerves();

    //restore connections from file
    if(restore) {
        FILE* saved=fopen("saved.txt", "r");
        for(int i=0;i<REC_NUM;i++) {
            for(int j=0;j<REC_NUM;j++) {
                fscanf(saved, "%f", &wrec[i][j]);
            }
        }
        fclose(saved);
    }

    //arguments for input fired at start (deprecated ... handled from settings and inside pulse function)
    threadArgs* a=malloc(sizeof(threadArgs));

    //arrays for plotting output1 (avgs1) and output2 (avgs2) if in statistic mode 
    double avgxs[50];
    double avgs1[50];
    double avgs2[50];

    // if statistic mode is enabled, random seeds will be tested and plotted
    if(statistic) {
        //increment seed and recreate neurons and connections
        for(int i=0;i<statisticInterval;i++) {
            srand(seed);
            createNerves();

            for(int i=0;i<OUT_NUM;i++) {
                avgOutBufferCount[i]=0;
                outputAvg[i]=0;
            }
                

            pthread_t thread_id1;
            pthread_create(&thread_id1, NULL, thread, (void *)a);

            pthread_join(thread_id1, NULL);
            seed++;
            tickCount=0;

            // iteration averages from statistics.h
            printf("%d 1: %f, 2: %f\n", seed, iterationAvg1, iterationAvg2);

            avgs1[i]=iterationAvg1;
            avgs2[i]=iterationAvg2;
            avgxs[i]=i;
            iterationAvg1=0;
            iterationAvg2=0;
        } 

        //plot collected averages of last 300 ticks of each seed simulated
        plot(avgxs, avgs1, avgs2, 50, "final");

    } else {
        //if not in statistic mode, only selected seed will be simulated
        pthread_t thread_id1;
        pthread_create(&thread_id1, NULL, thread, (void *)a);
    }

    //other threads if needed?
    /*
    pthread_t thread_id2;
    pthread_create(&thread_id2, NULL, thread, (void *)&b);

    usleep(300000);
    pthread_t thread_id3;
    pthread_create(&thread_id3, NULL, thread, (void *)&c);
    */

    //GLFW rendering loop
    while (!glfwWindowShouldClose(window) && tickCount<=plotSize && visualize) {
        glfwGetWindowSize(window, &width, &height);
        height = height > 0 ? height : 1; // Special case: avoid division by zero below
        glViewport(0, 0, width, height);

        // Clear color buffer to black
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND); 
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Select and setup the projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(65.f, (GLfloat) width / (GLfloat) height, 1.f, 100.f);

        // Select and setup the modelview matrix
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        gluLookAt(0.f, 1.f, 0.f,    // Eye-position
                  0.f, 20.f, 0.f,   // View-point
                  0.f, 0.f, 1.f);   // Up-vector

        glTranslatef(0.f, 20.f, 0.f);

        //-------------------------------------------------------------------------

        drawNerves();

        if(enableStatus)
            printStatus();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
