#include <unistd.h>
#include <pthread.h>

#include "nerve.h"
#include "pulse.h"

int randInt(int lower, int upper) {
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}


void createConnections() {
    //random connection generator
    /*
    for(int i=0;i<20;i++) {
        int connNum=randInt(1, 3);
        int valid=0;

//        Connection** conn=malloc(connNum*sizeof(Connection*));

        printf("%d: ", i);
        for(int c=0;c<connNum;c++) {
            int x, y;

            if(i%5==0) 
                x=randInt(0, 1);
            else if(i%5==4)
                x=randInt(-1, 0);
            else
                x=randInt(-1, 1);


            if(i<5)
                y=randInt(0, 1); 
            else if(i>14)
                y=randInt(-1, 0);
            else
                y=randInt(-1, 1);

            if(x!=0||y!=0) {
                wrec[i][i+y*5+x]=randInt(-3, 3);

                printf("%d(%f) ", i+y*5+x, wrec[i][i+y*5+x]);
            }
        }
        printf("\n");
        // nerves[i]->connectionNum=valid;
        // nerves[i]->connections=conn;
    }
    for(int i=0;i<4;i++) {
        printf("I%d: ", i);
        for(int j=0;j<4;j++) {
            win[i][j*5]=randInt(0, 3);
            if(win[i][j*5]>0)
                printf("%d(%f) ", j*5, win[i][j*5]);

        }
        printf("\n");
    }
    for(int i=4;i<20;i+=5) {
        printf("%d: ", i);
        for(int j=0;j<4;j++) {
            wout[i][j]=randInt(0, 3);
            if(wout[i][j]>0)
                printf("O%d(%f) ", j, wout[i][j]);

        }
        printf("\n");
    }
    */

    //all connected
    for(int i=0;i<inNum;i++) {
        win[0][0]=1.5f;
        win[0][5]=1.5f;
        win[0][10]=1.5f;

        win[1][5]=1.5f;
        win[1][10]=1.5f;
        win[1][15]=1.5f;
    }
    for(int i=4;i<recNum;i+=5) {
        wout[4][0]=1.5f;
        wout[9][0]=1.5f;
        wout[14][0]=1.5f;

        wout[9][1]=1.5f;
        wout[14][1]=1.5f;
        wout[19][1]=1.5f;
    }
    //for now feedforward
    for(int i=0;i<recNum;i++) {
        if((i-4)%5!=0 && i!=3) {
            if(i-5>=0) {
                wrec[i][i-5+1]=1.5f;
                printf("%d->%d ", i, i-5+1);
            }

            wrec[i][i+1]=1.5f;
            printf("%d->%d ", i, i+1);

            if(i+5<20) {
                wrec[i][i+5+1]=1.5f;
                printf("%d->%d\n", i, i+5+1);
            } else 
                printf("\n");

            //training
        } else if(i==3) {
           wrec[3][4]=2.5f; 
        }
    }
}

void createNerves() {
   int count=0;
   nerves=malloc(20*sizeof(Nerve*));
   for(int i=0;i<4;i++) {
        for(int j=0;j<5;j++) {
            Nerve* new=malloc(sizeof(Nerve));
            new->x=j*75-150;
            new->y=i*75-100;
            new->id=count;
            new->potential=0;

            new->multiplier=1;

            nerves[count]=new;
            count++;
        } 
   } 
   inputs=malloc(inNum*sizeof(Nerve*));
   for(int i=0;i<inNum;i++) {
        Nerve* new=malloc(sizeof(Nerve));
        new->x=-250;
        new->y=i*75-100;
        new->id=i;
        new->potential=0;

        new->multiplier=1;

        inputs[i]=new;
   }
   outputs=malloc(outNum*sizeof(Nerve*));
   for(int i=0;i<outNum;i++) {
        Nerve* new=malloc(sizeof(Nerve));
        new->x=250;
        new->y=i*75-100;
        new->id=i;
        new->potential=0;

        new->multiplier=1; 

        outputs[i]=new;
   }
   createConnections();
}



typedef struct _threadArgs{
    int len;
    int* inputs;
} threadArgs;

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

void drawNerves() {
    for(int i=0;i<20;i++) {
        glColor4f(1.f, 1.f, 1.f, 0.5f);
        if(nerves[i]->potential>20) {
            glColor4f(1.f, 0.f, 0.f, 1.f);
        }

        buildCircle(0.2f, 20, nerves[i]);
    }
    for(int i=0;i<inNum;i++) {
       glColor4f(0.f, 1.f, 1.f, 0.5f);
       if(inputs[i]->potential>20) {
            glColor4f(1.f, 0.f, 0.f, 1.f);
       }

       buildCircle(0.2f, 20, inputs[i]);
    }
    for(int i=0;i<outNum;i++) {
       glColor4f(1.f, 0.f, 1.f, 0.5f);
       if(outputs[i]->potential>20) {
            glColor4f(1.f, 0.f, 0.f, 1.f);
       }

       buildCircle(0.2f, 20, outputs[i]);
    }

    drawConnections();
}



int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Nervous", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    int width, height;
    double cx, cy;

    createNerves();

    threadArgs* a=malloc(sizeof(threadArgs));
    int aa[1]={0};
    a->inputs=aa;
    a->len=1;

    int b=1;
    int c=1;

    pthread_t thread_id1;
    pthread_create(&thread_id1, NULL, thread, (void *)a);

    /*
    pthread_t thread_id2;
    pthread_create(&thread_id2, NULL, thread, (void *)&b);

    usleep(300000);
    pthread_t thread_id3;
    pthread_create(&thread_id3, NULL, thread, (void *)&c);

    */


    while (!glfwWindowShouldClose(window)) {
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

        //glfwSetKeyCallback(window, keyPressed);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    pthread_join(thread_id1, NULL);

    /*
    pthread_join(thread_id2, NULL);

    pthread_join(thread_id3, NULL);
    */

    glfwTerminate();
    return 0;
}
