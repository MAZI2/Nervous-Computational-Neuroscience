/* adjust connections in iteration 
 *
 */

// desensitize neurons after firing
// resensitizing fatigued neurons every iteration
void adjustFatigue() {
    for(int i=0;i<REC_NUM;i++) {
        if(previousFired[i]) 
            neurons[i]->desensitize-=fatigue;
        else if(neurons[i]->desensitize<1.f) {
            neurons[i]->desensitize+=resensitizeRate;
        }
    }
    for(int i=0;i<OUT_NUM;i++) {
        if(outputs[i]->desensitize<1.f)
            outputs[i]->desensitize+=resensitizeRate;
    }
}

void adjustConnections() {
    FILE* saved=fopen("saved.txt", "w");
    if(saved==NULL) printf("error\n");

    //compare neurons to neurons that fired in last iteration
    //strengthen connection if successful (neuron that fired caused other to fire in next iteration)
    //decrease connection if unsuccessful
    for(int i=0;i<REC_NUM;i++) {
        if(!previousFired[i])
            continue;

        //adjust recurrent to recurrent
        for(int j=0;j<REC_NUM;j++) {
            if(wrec[i][j]>0) {
                if(currentFired[j]) {
                    wrec[i][j]+=pathAdjust;
                } else {
                    if(wrec[i][j]-pathAdjust<0)
                        wrec[i][j]=0;
                    else
                        wrec[i][j]-=pathAdjust;
                }
            }
        }

        //adjust output connections
        for(int j=0;j<OUT_NUM;j++) {
            if(wout[i][j]>0) {
                if(firedOutputs[j]) {
                    wout[i][j]+=pathAdjust;
                } else {
                    if(wout[i][j]-pathAdjust<0)
                        wout[i][j]=0;
                    else
                        wout[i][j]-=pathAdjust;
                }
            }
        }
    }

    //input neurons to recurrent are currently not adjusted
    
    //adjust input connections
    /*
    for(int i=0;i<IN_NUM;i++) {
        if(inputsFired[i]) {
            for(int j=0;j<currentFiredNum;j++) {
                int b=currentFired[j];
                if(win[i][b]>0)
                    win[i][b]+=0.1f;
            }

            //decrease input connections
            for(int j=0;j<REC_NUM;j++) {
                int success=0;
                for(int k=0;k<currentFiredNum;k++) {
                    if(j==currentFired[k]) {
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

    //save connections to file
    //TODO: should be togglable
    for(int i=0;i<REC_NUM;i++) {
        for(int j=0;j<REC_NUM;j++) {
           fprintf(saved, "%f ", wrec[i][j]); 
        }
    }
    
    fclose(saved);
}
