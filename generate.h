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
        win[0][8]=1.5f;
        win[0][16]=1.5f;

        win[1][16]=1.5f;
        win[1][24]=1.5f;
        win[1][32]=1.5f;
    }
    for(int i=4;i<recNum;i+=5) {
        wout[7][0]=1.5f;
        wout[15][0]=1.5f;
        wout[23][0]=1.5f;

        wout[23][1]=1.5f;
        wout[31][1]=1.5f;
        wout[39][1]=1.5f;
    }
    //for now feedforward
    for(int i=0;i<recNum;i++) {
        if((i-7)%8!=0 && i!=4 && i!=5) {
            if(i-8>=0) {
                wrec[i][i-8+1]=1.5f;
                printf("%d->%d ", i, i-8+1);
            }

            wrec[i][i+1]=1.5f;
            printf("%d->%d ", i, i+1);

            if(i+8<40) {
                wrec[i][i+8+1]=1.5f;
                printf("%d->%d\n", i, i+8+1);
            } else 
                printf("\n");

            //training
        } else if(i==4) {
           wrec[4][5]=4.5f; 
           wrec[4][13]=3.5f;
        } else if(i==5) {
           wrec[5][6]=4.5f;
           wrec[5][14]=3.5f;
        }
    }
}

void createNerves() {
   int count=0;
   nerves=malloc(recNum*sizeof(Nerve*));
   for(int i=0;i<5;i++) {
        for(int j=0;j<8;j++) {
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
        new->x=550;
        new->y=i*75-100;
        new->id=i;
        new->potential=0;

        new->multiplier=1; 

        outputs[i]=new;
   }
   createConnections();
}
