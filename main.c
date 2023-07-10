#include "nerve.h"

static Nerve** nerves;


int randInt(int lower, int upper) {
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

void createConnections() {
    for(int i=0;i<20;i++) {
        int connNum=randInt(1, 3);
        int valid=0;

        Nerve** conn=malloc(connNum*sizeof(Nerve*));

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

            conn[valid]=nerves[i+y*5+x];
            printf("%d ", i+y*5+x);

            valid++;

           /* 
            if(i+x>=0 && i+y*5>=0 && i+y*5+x!=i && i+5*y+x<20) {
                conn[valid]=nerves[i+y*5+x];
                printf("%d ", i+y*5+x);

                valid++;
            }
            */
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

            nerves[count]=new;
            count++;

        } 
   } 

   createConnections();
}



void drawNerves() {
    glColor4f(0.f, 1.f, 1.f, 1.f);
    for(int i=0;i<20;i++) {
        if(i==4)
            glColor4f(1.f, 0.f, 0.f, 1.f);
        buildCircle(0.3f, 20, nerves[i]);
        glColor4f(0.f, 1.f, 1.f, 1.f);
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

    while (!glfwWindowShouldClose(window)) {
        glfwGetWindowSize(window, &width, &height);
        height = height > 0 ? height : 1; // Special case: avoid division by zero below
        glViewport(0, 0, width, height);

        // Clear color buffer to black
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND); 
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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
