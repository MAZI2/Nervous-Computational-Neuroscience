int path[100];
int ix=0;

void adjustOutgoing(int goalOutput) {
    //rec to rec
    //start to end
    for(int i=ix-1;i>=0;i--) {
        for(int j=0;j<recNum;j++) {
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
            for(int j=0;j<outNum;j++) {
                if(wout[path[i]][j]>0 && j!=goalOutput)
                    wout[path[i]][j]=0.3f;
            }
        }
    }
}

int* temp;
int tempix;

void findPath(Nerve* goal, int isOutput, int goalInput) {

    if(isOutput) {
        tempix=ix;
        temp=malloc(ix*sizeof(int));
        for(int i=0;i<ix;i++) {
            temp[i]=path[i];
        }

        ix=0;
    }

    for(int i=0;i<inNum;i++) {
       if(win[i][goal->id]>0 && !isOutput && i==goalInput) {
            win[i][goal->id]=3.f;
            printf(" <- %dI\n", i);

            return;
       }
    }
    for(int i=0;i<recNum;i++) {
        if(isOutput==1) {
            if(nerves[i]->neuronType==1 && wout[i][goal->id]>0) {
                wout[i][goal->id]=3.f;
                printf("%d <- %d", goal->id, i);

                path[ix]=i;
                ix++;
                findPath(nerves[i], 0, goalInput);
                break;
            }
        } else if (nerves[i]->neuronType==1 && wrec[i][goal->id]>0) {
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
                findPath(nerves[i], 0, goalInput); 
                break;
            }
        }
    }
    
    return;
}
