int randInt(int lower, int upper) {
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

void createConnections() {
    //inputs randomly connected to left half of recurrent
    for(int i=0;i<inNum;i++) {
        for(int j=0;j<recNum;j++) {
            if((j%10)<5) {
                int rnd=randInt(0, 2);
                if(rnd==0)
                    win[i][j]=1.5f;
            }
        }
    }
    //outputs randomly connected to right half of recurrent
    for(int i=0;i<outNum;i++) {
        for(int j=0;j<recNum;j++) {
            if((j%10)>5) {
                int rnd=randInt(0, 1);
                if(rnd==0)
                    //printf("this\n");
                    wout[j][i]=1.5f;
            }
        }
    }

    for(int i=0;i<recNum;i++) {
        //feedforward
        /*
        if((i-recX+1)%recX!=0) {
            if(i-recX>=0) {
                wrec[i][i-recX+1]=1.5f;
                if((i-recX+2)%recX!=0)
                    wrec[i][i-recX+2]=1.5f;
                //printf("%d->%d ", i, i-recX+1);
            }

            wrec[i][i+1]=1.5f;
            //printf("%d->%d ", i, i+1);

            if(i+recX<recNum) {
                wrec[i][i+recX+1]=1.5f;
                if((i+recX+1)<recNum && (i+recX+2)%recX!=0)
                    wrec[i][i+recX+2]=1.5f;

                //printf("%d->%d\n", i, i+recX+1);
            } else {}
                //printf("\n");

        } 
        */
        //recurrent
        // TODO: inefficient ... adjacency matrix would be better
        for(int j=0;j<recNum;j++) {
            if(j!=i) {
                int a=randInt(0, 10);
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
   nerves=malloc(recNum*sizeof(Nerve*));
   for(int i=0;i<recY;i++) {
        for(int j=0;j<recX;j++) {
            Nerve* new=malloc(sizeof(Nerve));
            new->x=j*50-150;
            new->y=i*50-230;
            new->id=count;
            new->potential=0;

            new->neuronType=1;
            new->multiplier=1.f;
            new->desensitize=1.f;

            int type=randInt(0, 2);
            if(type==0) {
                new->neuronType=-1;
                new->multiplier=-1.f;
            }

            new->receptorType=1;
            int receptor=randInt(0, 1);
            if(receptor==0)
                new->receptorType=-1;

            nerves[count]=new;
            count++;
        } 
   } 
   inputs=malloc(inNum*sizeof(Nerve*));
   for(int i=0;i<inNum;i++) {
        Nerve* new=malloc(sizeof(Nerve));
        new->x=-250;
        new->y=i*200-100;
        new->id=i;
        new->potential=0;

        new->neuronType=1;
        new->receptorType=1;
        new->multiplier=1.f;
        new->desensitize=1.f;

        inputs[i]=new;
   }
   outputs=malloc(outNum*sizeof(Nerve*));
   for(int i=0;i<outNum;i++) {
        Nerve* new=malloc(sizeof(Nerve));
        new->x=400;
        new->y=i*200-100;
        new->id=i;
        new->potential=0;

        new->neuronType=1;
        new->receptorType=1;
        new->multiplier=1.f; 
        new->desensitize=1.f;

        outputs[i]=new;
   }

   createConnections();
}
