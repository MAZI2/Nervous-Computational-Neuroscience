#define GLFW_INCLUDE_GLU 
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <math.h>

typedef struct _Point {
    float x, y, z;
} Point;

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

void buildCircleOutline(float radius, int vCount, Nerve* nerve) {
    float cx=nerve->x;
    float cy=nerve->y;

    float angle=(2*M_PI)/vCount;
    float tangetial_factor=tanf(angle);
    float radial_factor=cosf(angle);

    float x=radius;
    float y=0;

    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<vCount;i++) {
        glVertex3f(x + cx/20, 0.f, y - cy/20);

        float tx=-y;
        float ty=x;

        x+=tx*tangetial_factor;
        y+=ty*tangetial_factor;

        x*=radial_factor;
        y*=radial_factor;
    }

    glEnd();
}

void line(Nerve* start, Nerve* end, float width, int output) {
    float xa=start->x/20;
    float ya=-(start->y/20);
    float xb=end->x/20;
    float yb=-end->y/20;

    if(start->potential>threshold) {
        glColor4f(1.f, 0.f, 0.f, 0.5f*width);
    } else {
        if(start->receptorType==-1)
            glColor4f(1.f, 1.f/dopamine, 1.f, 0.5f*width);
        else
            glColor4f(1.f/dopamine, 1.f, 1.f, 0.5f*width);

    }
    int both=0;
    for(int j=0;j<ix;j++) {
        if(path[j]==start->id)
            both++;
        if(path[j]==end->id)
            both++;

        if(both==2) {
            glColor4f(1.f, 1.f, 0.f, 0.5f*width);
        }
    }


    glLineWidth(1);

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
    //recurrent
    for(int i=0;i<recNum;i++) {
       for(int j=0;j<recNum;j++) {
            if(wrec[i][j]>0) {
                line(nerves[i], nerves[j], wrec[i][j], 0);
            }
       }
    }
    //inputs
    for(int i=0;i<inNum;i++) {
        for(int j=0;j<recNum;j++) {
            if(win[i][j]>0) {
                line(inputs[i], nerves[j], win[i][j], 0);
            }
        }
    }
    //outputs
    for(int i=0;i<outNum;i++) {
        for(int j=0;j<recNum;j++) {
            if(wout[j][i]>0) {
     //           if(j==7 && i==1)
                    //printf("%f\n", wout[7][1]);
                line(nerves[j], outputs[i], wout[j][i], 1);
            }
        }
    }
}

void drawNerves() {

    drawConnections();
    for(int i=0;i<recNum;i++) {

        //default color
        glColor4f(1.f, 1.f/dopamine, 1.f, 0.5f);
        //negative d response color
        if(nerves[i]->receptorType==-1)
            glColor4f(1.f, 1.f/dopamine, 1.f, 0.5f);
        else
            glColor4f(1.f/dopamine, 1.f, 1.f, 0.5f);


        //red color for fired
        if(nerves[i]->potential>threshold) {
            glColor4f(1.f, 0.f, 0.f, 0.5f);
        }

        //circle or outline (neuron type)
        if(nerves[i]->neuronType==-1) {
            buildCircleOutline(0.2f, 20, nerves[i]);
            glColor4f(0.f, 0.f, 0.f, 1.f);
            buildCircle(0.15f, 20, nerves[i]);

        } else {
            for(int j=0;j<ix;j++) {
                if(path[j]==i) {
                    glColor4f(1.f, 1.f, 0.f, 1.f);
                    break;
                }   
            }
            buildCircle(0.2f, 20, nerves[i]);
        }
    }
    for(int i=0;i<inNum;i++) {
       glColor4f(0.f, 1.f, 1.f, 0.5f);
       if(inputs[i]->potential>threshold) {
            glColor4f(1.f, 0.f, 0.f, 1.f);
       }

       buildCircle(0.2f, 20, inputs[i]);
    }
    for(int i=0;i<outNum;i++) {
       glColor4f(1.f, 1.f, 1.f, 0.5f);
       if(outputs[i]->potential>threshold) {
            glColor4f(1.f, 0.f, 0.f, 1.f);
       }

       buildCircle(0.2f, 20, outputs[i]);
    }

}
