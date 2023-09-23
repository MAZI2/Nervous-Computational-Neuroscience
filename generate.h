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
        win[0][10]=1.5f;
        win[0][20]=1.5f;
        win[0][30]=1.5f;
        win[0][40]=1.5f;

        win[1][50]=1.5f;
        win[1][60]=1.5f;
        win[1][70]=1.5f;
        win[1][80]=1.5f;
        win[1][90]=1.5f;


    }
    for(int i=4;i<recNum;i+=5) {
        wout[9][0]=1.5f;
        wout[19][0]=1.5f;
        wout[29][0]=1.5f;
        wout[39][0]=1.5f;
        wout[49][0]=1.5f;

        wout[59][1]=1.5f;
        wout[69][1]=1.5f;
        wout[79][1]=1.5f;
        wout[89][1]=1.5f;
        wout[99][1]=1.5f;
    }
    //for now feedforward
    for(int i=0;i<recNum;i++) {
        if((i-recX+1)%recX!=0) {
            if(i-recX>=0) {
                wrec[i][i-recX+1]=1.5f;
                //printf("%d->%d ", i, i-recX+1);
            }

            wrec[i][i+1]=1.5f;
            //printf("%d->%d ", i, i+1);

            if(i+recX<recNum) {
                wrec[i][i+recX+1]=1.5f;
                //printf("%d->%d\n", i, i+recX+1);
            } else {}
                //printf("\n");

        } 

    }
    
   //training
   /* wrec[28][37]=6.5f; */
   /* wrec[28][29]=6.5f; */
   /* wrec[38][31]=6.5f; */
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

            int type=randInt(0, 3);
            if(type==0) {
                //printf("%d\n", i*8+j);
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

        outputs[i]=new;
   }
   createConnections();
}
