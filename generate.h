#include "PathAlgorithm.h"

int randInt(int lower, int upper) {
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

void createConnections() {
    //inputs randomly connected to left half of recurrent
    for(int i=0;i<IN_NUM;i++) {
        for(int j=0;j<REC_NUM;j++) {
            if((j%REC_X)<REC_X/2) {
                int rnd=randInt(0, 10);
                if(rnd==0)
                    win[i][j]=1.5f;
            }
        }
    }
    //outputs randomly connected to right half of recurrent
    for(int i=0;i<OUT_NUM;i++) {
        for(int j=0;j<REC_NUM;j++) {
            if((j%REC_X)<REC_X/2) {
                int rnd=randInt(0, 10);
                if(rnd==0)
                    //printf("this\n");
                    wout[j][i]=1.5f;
            }
        }
    }

    for(int i=0;i<REC_NUM;i++) {
        //feedforward
        /*
        if((i-REC_X+1)%REC_X!=0) {
            if(i-REC_X>=0) {
                wrec[i][i-REC_X+1]=1.5f;
                if((i-REC_X+2)%REC_X!=0)
                    wrec[i][i-REC_X+2]=1.5f;
                //printf("%d->%d ", i, i-REC_X+1);
            }

            wrec[i][i+1]=1.5f;
            //printf("%d->%d ", i, i+1);

            if(i+REC_X<REC_NUM) {
                wrec[i][i+REC_X+1]=1.5f;
                if((i+REC_X+1)<REC_NUM && (i+REC_X+2)%REC_X!=0)
                    wrec[i][i+REC_X+2]=1.5f;

                //printf("%d->%d\n", i, i+REC_X+1);
            } else {}
                //printf("\n");

        } 
        */
        //recurrent
        // TODO: inefficient ... adjacency matrix would be better
        for(int j=0;j<REC_NUM;j++) {
            if(j!=i) {
                int a=randInt(0, 50);
                if(a==10) {
                    wrec[i][j]=1.5f;
                }
            }

        }
    }
    
   //for association training
   /* wrec[28][37]=6.5f; */
   /* wrec[28][29]=6.5f; */
   /* wrec[38][31]=6.5f; */

   //if paths are artificially strengthened
   if(pretrained) {
       //find path from 0->0
       findPath(outputs[0], 1, 0);
       adjustOutgoing(0);

       //find path from 1->1
       findPath(outputs[1], 1, 1);
       adjustOutgoing(1);
   }
}

void createNerves() {
   int count=0;
   neurons=malloc(REC_NUM*sizeof(Nerve*));
   for(int i=0;i<REC_Y;i++) {
        for(int j=0;j<REC_X;j++) {
            Nerve* new=malloc(sizeof(Nerve));
            new->x=j*25-150;
            new->y=i*25-230;
            new->id=count;
            new->potential=0;

            new->neuronType=1;
            new->desensitize=1.f;

            int type=randInt(0, 2);
            if(type==0) {
                new->neuronType=-1;
            }

            new->receptorType=1;
            int receptor=randInt(0, 1);
            if(receptor==0)
                new->receptorType=-1;

            neurons[count]=new;
            count++;
        } 
   } 
   inputs=malloc(IN_NUM*sizeof(Nerve*));
   for(int i=0;i<IN_NUM;i++) {
        Nerve* new=malloc(sizeof(Nerve));
        new->x=-250;
        new->y=i*200-100;
        new->id=i;
        new->potential=0;

        new->neuronType=1;
        new->receptorType=1;
        new->desensitize=1.f;

        inputs[i]=new;
   }
   outputs=malloc(OUT_NUM*sizeof(Nerve*));
   for(int i=0;i<OUT_NUM;i++) {
        Nerve* new=malloc(sizeof(Nerve));
        new->x=400;
        new->y=i*200-100;
        new->id=i;
        new->potential=0;

        new->neuronType=1;
        new->receptorType=1;
        new->desensitize=1.f;

        outputs[i]=new;
   }

   createConnections();
}
