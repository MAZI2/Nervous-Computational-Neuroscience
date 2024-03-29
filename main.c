#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//network specifications
#include "settings.h"

#include "PathAlgorithm.h"
//draw
#include "nerve.h"
//generate neurons and connections
#include "generate.h"

#include "statistics.h"
#include "pulse.h"

typedef struct _threadArgs{
    int len;
    int* inputs;
} threadArgs;

//currently everything on one thread
void *thread(void *vargp) {
        int t;

        threadArgs* args=(threadArgs*)vargp;
        int len=args->len;
        int* inputsArr=args->inputs;

        for(int i=0;i<len;i++) {
            inputs[inputsArr[i]]->potential=22;
        }
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

    // TODO: display for any size of output array 
    printf("Output 1 activity: %f\n", output1count);
    printf("Output 2 activity: %f\n", output2count);
    // --
    printf("Output 1 potential: %f\n", outputs[0]->potential);
    printf("Output 2 potential: %f", outputs[1]->potential);


    printf("\n");
    printf("Dopamine: %f\n", dopamine);
    printf("Decay: %f\n", decay);

    printf("I1:%d  O1%d\n", inputsFired[0], outputsFired[0]);
    printf("I2:%d  O2%d\n", inputsFired[1], outputsFired[1]);

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

    //display of output fires over 10ticks and the sum
    int sum1=0;
    int sum2=0;
    for(int i=0;i<10;i++) {
        if(avgOut1[i]) sum1++;
        printf("%d ", avgOut1[i]);
    }
    printf("%d\n", sum1);


    for(int i=0;i<10;i++) {
        if(avgOut2[i]) sum2++;
        printf("%d ", avgOut2[i]);
    }
    printf("%d\n", sum2);


    //move cursor up 24 lines
    for(int i=0;i<24;i++)
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


    createNerves();

    //restore connections from file
    if(restore) {
        FILE* saved=fopen("saved.txt", "r");
        for(int i=0;i<recNum;i++) {
            for(int j=0;j<recNum;j++) {
                fscanf(saved, "%f", &wrec[i][j]);
            }
        }
        fclose(saved);
    }

    //arguments for input fired at start (deprecated ... handled from settings and inside pulse function)
    threadArgs* a=malloc(sizeof(threadArgs));
    int aa[1]={1};
    a->inputs=aa;
    a->len=1;

    int b=1;
    int c=1;

    //arrays for plotting output1 (avgs1) and output2 (avgs2) if in statistics mode 
    double avgxs[50];
    double avgs1[50];
    double avgs2[50];

    if(statistic) {
        //increment seed and recreate neurons and connections
        for(int i=0;i<statisticInterval;i++) {
            srand(seed);
            createNerves();

            pthread_t thread_id1;
            pthread_create(&thread_id1, NULL, thread, (void *)a);

            pthread_join(thread_id1, NULL);
            seed++;
            tickCount=0;

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

        /*
        double t = glfwGetTime();
        glRotatef(0.3f * (GLfloat) x + (GLfloat) t * 100.f, 0.f, 0.f, 1.f);
        */

        //-------------------------------------------------------------------------

        drawNerves();

        if(enableStatus)
            printStatus();

        //glfwSetKeyCallback(window, keyPressed);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*
    if(!statistic)
        pthread_join(thread_id1, NULL);
    */

    /*
    pthread_join(thread_id2, NULL);

    pthread_join(thread_id3, NULL);
    */

    glfwTerminate();
    return 0;
}
