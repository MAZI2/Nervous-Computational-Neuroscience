#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>


typedef struct _Point {
    float x, y, z;
} Point;


static const Point* points;

/*
void buildCircle(float radius, int vCount) {
    float angle=(2*Math.PI)/vCount;
    int triangleCount=vCount-2;

    points=malloc()

    Verte

    for(int i=0;i<vCount;i++) {
        float currentAngle=angle*i;
        float x=radius*cos(currentAngle); 
        float y=radius*sin(currentAngle);
        float z=0.0f; 


    }
}
*/

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
    glClearColor( 0.2f, 0.3f, 0.4f, 0.0f );

    /*
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);


        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-5.f, 0.f, -4.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(5.f, 0.f, -4.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.f, 6.f);
        glEnd();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    */

    int width, height;
    double x;

    while (!glfwWindowShouldClose(window)) {

        double t = glfwGetTime();
        glfwGetCursorPos(window, &x, NULL);

        // Get window size (may be different than the requested size)
        glfwGetWindowSize(window, &width, &height);

        // Special case: avoid division by zero below
        height = height > 0 ? height : 1;

        glViewport(0, 0, width, height);

        // Clear color buffer to black
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

        // Draw a rotating colorful triangle
        glTranslatef(0.f, 14.f, 0.f);
        /*
        glRotatef(0.3f * (GLfloat) x + (GLfloat) t * 100.f, 0.f, 0.f, 1.f);
        */

        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-5.f, 0.f, -4.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(5.f, 0.f, -4.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.f, 6.f);
        glEnd();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }


    glfwTerminate();
    return 0;
}
