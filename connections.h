int prev[100];
int fired=0;
//new fires
int tempPrev[100];
int tempFired=0;

int outputsFired[2];
int inputsFired[2];

void resensitize() {
    for(int i=0;i<tempFired;i++) {
        int a=tempPrev[i];
        nerves[a]->desensitize-=fatigue;
    }
    for(int i=0;i<recNum;i++) {
        int d=nerves[i]->desensitize;
        if(d<1.f)
            nerves[i]->desensitize+=0.1f;
    }
    for(int i=0;i<outNum;i++) {
        int d=outputs[i]->desensitize;
        if(d<1.f)
            outputs[i]->desensitize+=0.1f;
    }
}

void adjustConnections() {
    FILE* saved=fopen("saved.txt", "w");
    if(saved==NULL) printf("error\n");

    for(int i=0;i<fired;i++) {
        for(int j=0;j<tempFired;j++) {
            int a=prev[i];
            int b=tempPrev[j];
            if(wrec[a][b]>0) {
                wrec[a][b]+=pathAdjust;
    //            printf("Strength %d -> %d + 0.05\n", a, b);
            } 
        }
        for(int j=0;j<outNum;j++) {
            if(outputsFired[j]) {
                int a=prev[i];
                if(wout[a][j]>0) {
                    wout[a][j]+=pathAdjust;
                }
            }
        }
        //inneficient decrease recurrent to recurrent
        for(int j=0;j<recNum;j++) {
            int successful=0;
            for(int k=0;k<tempFired;k++) {
                if(j==tempPrev[k]) {
                    successful=1;
                    break;
                }
            }

            int a=prev[i];

            if(!successful && wrec[a][j]>0) {
                if(wrec[a][j]-pathAdjust<0)
                    wrec[a][j]=0;
                else
                    wrec[a][j]-=pathAdjust;
   //                 printf("Decrease %d -> %d - 0.05\n", a, j);
            }
        }

        //decrease output connections
        for(int j=0;j<outNum;j++) {
            int a=prev[i];
            if(!outputsFired[j] && wout[a][j]>0) {
                if(wout[a][j]-pathAdjust<0)
                    wout[a][j]=0;
                else
                    wout[a][j]-=pathAdjust;
            }
        }

    }
    //adjust input connections
    /*
    for(int i=0;i<inNum;i++) {
        if(inputsFired[i]) {
            for(int j=0;j<tempFired;j++) {
                int b=tempPrev[j];
                if(win[i][b]>0)
                    win[i][b]+=0.1f;
            }

            //decrease input connections
            for(int j=0;j<recNum;j++) {
                int success=0;
                for(int k=0;k<tempFired;k++) {
                    if(j==tempPrev[k]) {
                        success=1;
                        break;
                    }
                }
                if(!success && win[i][j]>0) {
                    if(win[i][j]-0.1f<0)
                        win[i][j]=0;
                    else
                        win[i][j]-=0.1f;
                }
            }
        }
    }
    */

    for(int i=0;i<tempFired;i++) {
        prev[i]=tempPrev[i];
    }
    fired=tempFired;

    for(int i=0;i<recNum;i++) {
        for(int j=0;j<recNum;j++) {
           fprintf(saved, "%f ", wrec[i][j]); 
        }
    }
    
    fclose(saved);
    resensitize();
}
