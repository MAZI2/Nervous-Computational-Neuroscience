// THIS NEEDS TO BE UPDATED!!

//array where found path is stored
int path[100];
int ix=0;

//outgoing neurons from found path are weakened to prevent a lot of recurrent action (possibily a lot of wrong output)
void adjustOutgoing(int goalOutput) {
    //rec to rec
    //start to end
    for(int i=ix-1;i>=0;i--) {
        for(int j=0;j<REC_NUM;j++) {
            if(i>0 && j!=path[i-1]) {
                if(wrec[path[i]][j]>0) {
                    wrec[path[i]][j]=0.3f;
                }
            } else if(i==0) {
                if(wrec[path[i]][j]>0) {
                    wrec[path[i]][j]=0.3f;
                }
            }
        }
        if(i==0) {
            for(int j=0;j<OUT_NUM;j++) {
                if(wout[path[i]][j]>0 && j!=goalOutput)
                    wout[path[i]][j]=0.3f;
            }
        }
    }
}

int* temp;
int tempix;

//find a path from goal output neuron, to goalInput neuron
//search excitatory paths
//dumb function args ... goal is next neuron, isOuput marks if goal is output, goalInput is target input number
void findPath(Nerve* goal, int isOutput, int goalInput) {

    if(isOutput) {
        tempix=ix;
        temp=malloc(ix*sizeof(int));
        for(int i=0;i<ix;i++) {
            temp[i]=path[i];
        }

        ix=0;
    }

    for(int i=0;i<IN_NUM;i++) {
       if(win[i][goal->id]>0 && !isOutput && i==goalInput) {
            win[i][goal->id]=3.f;
            printf(" <- %dI\n", i);

            return;
       }
    }
    for(int i=0;i<REC_NUM;i++) {
        if(isOutput==1) {
            if(neurons[i]->neuronType==1 && wout[i][goal->id]>0) {
                wout[i][goal->id]=3.f;
                printf("%d <- %d", goal->id, i);

                path[ix]=i;
                ix++;
                findPath(neurons[i], 0, goalInput);
                break;
            }
        } else if (neurons[i]->neuronType==1 && wrec[i][goal->id]>0) {
            int forbidden=0;
            for(int j=0;j<tempix;j++) {
                if(i==temp[j]) {
                    forbidden=1;
                    break;
                }
            }
            if(!forbidden) {
                wrec[i][goal->id]=3.f;
                printf(" <- %d", i);

                path[ix]=i;
                ix++;
                findPath(neurons[i], 0, goalInput); 
                break;
            }
        }
    }
    
    return;
}
