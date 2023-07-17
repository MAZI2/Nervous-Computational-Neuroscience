#include "nerve.h"
#include <unistd.h>
#include <pthread.h>

static Nerve** nerves;

Nerve** activeBuffer[5];
int activeNumsBuffer[5];


int randInt(int lower, int upper) {
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

void createConnections() {
    for(int i=0;i<20;i++) {
        int connNum=randInt(1, 3);
        int valid=0;

        Connection** conn=malloc(connNum*sizeof(Connection*));

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
                Connection* newConnection=malloc(sizeof(Connection));
                newConnection->end=nerves[i+y*5+x];
                newConnection->strength=1;

                conn[valid]=newConnection;
                printf("%d ", i+y*5+x);

                valid++;
            }
        }
        printf("\n");
        nerves[i]->connectionNum=valid;
        nerves[i]->connections=conn;
    }
}

void createNerves() {
   int count=0;
   nerves=malloc(20*sizeof(Nerve*));
   for(int i=0;i<4;i++) {
        for(int j=0;j<5;j++) {
            Nerve* new=malloc(sizeof(Nerve));
            new->x=j*75-100;
            new->y=i*75-100;
            new->id=count;

            nerves[count]=new;
            count++;
        } 
   } 

   createConnections();
}

void sendPulse(Nerve** activeNerves, int* activeNum) {
    usleep(500000);

    if(*activeNum>0) {
       int count=0;
       Nerve* tempNerves[20];

       //for all active of current path count sum of connections from them
       for(int i=0;i<*activeNum;i++) {

           //add all connected neurons to temporary array and make it new active
           for(int j=0;j<activeNerves[i]->connectionNum;j++) {
               int alreadyActive=0;
               for(int z=0;z<count;z++) {
                    if(tempNerves[z]==activeNerves[i]->connections[j]->end) {
                        alreadyActive=1; 
                        break;
                    }
               }
               if(!alreadyActive) {
                    tempNerves[count]=activeNerves[i]->connections[j]->end;

                    printf("%d->%d\n", activeNerves[i]->id, activeNerves[i]->connections[j]->end->id);
                    count++;
               }
           }      
       }
       //set new number of active points and fill the new active array from temporary
       *activeNum=count;
       for(int i=0;i<*activeNum;i++) {
           activeNerves[i]=tempNerves[i]; 
       }
       printf("----\n");

//       printf("%d\n", *activeNum);
       sendPulse(activeNerves, activeNum);
   }
}


void *thread(void *vargp) {
        int t;
        int* key=(int *)vargp;
        printf("%d\n", *key);

        for(int i=0;i<5;i++) {
            if(activeBuffer[i]==NULL) {
                activeBuffer[i]=malloc(20*sizeof(Nerve*));
                activeBuffer[i][0]=nerves[*key];
                activeNumsBuffer[i]=1;

                t=i;

                break; 
            }
        }
        sendPulse(activeBuffer[t], &activeNumsBuffer[t]);

        free(activeBuffer[t]);
        activeBuffer[t]=NULL;


    return NULL;
}

/*
void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods) {  
    int keyNum=key-'0';
    if(action==GLFW_PRESS) {

    }  
}
*/

void drawNerves() {
    glColor4f(1.f, 1.f, 1.f, 1.f);
    for(int i=0;i<20;i++) {
        for(int b=0;b<5;b++) {
            for(int j=0;j<activeNumsBuffer[b];j++) {
                if(nerves[i]==activeBuffer[b][j]) {
                    glColor4f(1.f, 0.f, 0.f, 1.f);
                } 
            }
        }

        buildCircle(0.2f, 20, nerves[i]);
        glColor4f(1.f, 1.f, 1.f, 0.5f);

    }
}

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "GLFW CMake starter", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    int width, height;
    double cx, cy;

    createNerves();

    //thread for every path
    for(int i=0;i<5;i++) activeBuffer[i]=NULL;

    int a=10;
    int b=19;
    int c=1;

    pthread_t thread_id1;
    pthread_create(&thread_id1, NULL, thread, (void *)&a);

    /*
    usleep(200000);
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
