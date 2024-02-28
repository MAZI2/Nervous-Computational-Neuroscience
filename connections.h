//array of fired neurons from previous iteration
// TODO: variable names and fixed numbers - bad
int prev[100];
int fired=0;
//array of fired in the last iteration
int tempPrev[100];
int tempFired=0;

int outputsFired[2];
int inputsFired[2];


// desensitize neurons after firing
//resensitizing fatigued neurons every iteration
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

    //compare neurons to neurons that fired in last iteration
    //strengthen connection if successful (neuron that fired caused other to fire in next iteration)
    //decrease connection if unsuccessful
    for(int i=0;i<fired;i++) {
        for(int j=0;j<tempFired;j++) {
            int a=prev[i];
            int b=tempPrev[j];
            if(wrec[a][b]>0) {
                wrec[a][b]+=pathAdjust;
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
        //decrease recurrent to recurrent
        //very inefficient
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
    //input neurons to recurrent are currently not adjusted
    
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

    //swap prev with current
    for(int i=0;i<tempFired;i++) {
        prev[i]=tempPrev[i];
    }
    fired=tempFired;

    //save connections to file
    //TODO: should be togglable
    for(int i=0;i<recNum;i++) {
        for(int j=0;j<recNum;j++) {
           fprintf(saved, "%f ", wrec[i][j]); 
        }
    }
    
    fclose(saved);
    resensitize();
}
