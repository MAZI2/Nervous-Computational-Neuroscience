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
    int id;
    float potential;
} Nerve;

static Nerve** inputs;
static Nerve** nerves;
static Nerve** outputs;

float win[4][20];
float wrec[20][20];
float wout[20][4];


void buildCircle(float radius, int vCount, Nerve* nerve) {
    float cx=nerve->x;
    float cy=nerve->y;

    //drawing a circle
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

    free(points);
}

void line(Nerve* start, Nerve* end, float width) {
    float xa=start->x/20;
    float ya=-(start->y/20);
    float xb=end->x/20;
    float yb=-end->y/20;
    
    if(start->potential>20) {
        glColor4f(1.f, 0.f, 0.f, 1.f);
    } else
        glColor4f(1.f, 1.f, 1.f, 0.5f);

    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex3f(xa, 0.f, ya);
    glVertex3f(xb, 0.f, yb);
    glEnd();

    float px=0.9f*(xb-xa)+xa;
    float py=0.9f*(yb-ya)+ya;

    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex3f(px, 0.f, py);
    glEnd();
}

void drawConnections() {
    for(int i=0;i<20;i++) {
       for(int j=0;j<20;j++) {
            if(wrec[i][j]>0) {
                line(nerves[i], nerves[j], wrec[i][j]);
            }
       }
    }
    for(int i=0;i<4;i++) {
        for(int j=0;j<20;j++) {
            if(win[i][j]>0) {
                line(inputs[i], nerves[j], win[i][j]);
            }
        }
    }
    for(int i=0;i<20;i++) {
        for(int j=0;j<4;j++) {
            if(wout[i][j]>0) {
                line(nerves[i], outputs[j], wout[i][j]);
            }
        }
    }
}
