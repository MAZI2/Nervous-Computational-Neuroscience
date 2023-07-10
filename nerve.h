#define GLFW_INCLUDE_GLU 
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _Point {
    float x, y, z;
} Point;

typedef struct _Nerve {
    float x, y;
    struct _Nerve** connections;
    int connectionNum;
} Nerve;

void buildCircle(float radius, int vCount, Nerve* nerve) {
    float cx=nerve->x;
    float cy=nerve->y;

    Point** points=malloc(vCount*sizeof(Point));

    float angle=(2*M_PI)/vCount;
    int triangleCount=vCount-2;

    for(int i=0;i<vCount;i++) {
        float currentAngle=angle*i;
        float x=radius*cos(currentAngle) + cx/20;
        float y=radius*sin(currentAngle) - cy/20;
        float z=0.0f;

        Point* point=malloc(sizeof(Point));
        point->x=x;
        point->y=y;
        point->z=z;

        points[i]=point;
    }

    for(int i=0;i<=triangleCount;i++) {
        glBegin(GL_TRIANGLES);


        glVertex3f(points[0]->x, points[0]->z, points[0]->y);
        glVertex3f(points[i]->x, points[i]->z, points[i]->y);
        glVertex3f(points[i+1]->x, points[i+1]->z, points[i+1]->y);

        glEnd();
    }
    Nerve** connections=nerve->connections;

    for(int i=0;i<nerve->connectionNum;i++) {
//       printf("%f %f -> %f %f\n", nerve->x, nerve->y, connections[i]->x, connections[i]->y);

       glBegin(GL_LINES);
       glVertex3f(nerve->x/20, 0.f, (nerve->y/20)-1);
       glVertex3f(connections[i]->x/20, 0.f, (connections[i]->y/20)-1);
       glEnd();
    }

    free(points);
}
