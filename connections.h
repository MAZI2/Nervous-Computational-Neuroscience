int prev[100];
int fired=0;
//new fires
int tempPrev[100];
int tempFired=0;

void resensitize() {
    for(int i=0;i<tempFired;i++) {
        int a=tempPrev[i];
        nerves[a]->desensitize-=0.3f;
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
                wrec[a][b]+=0.1f;
    //            printf("Strength %d -> %d + 0.05\n", a, b);
            } 
        }
        //inneficient
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
                if(wrec[a][j]-0.1f<0)
                    wrec[a][j]=0;
                else
                    wrec[a][j]-=0.1f;
   //                 printf("Decrease %d -> %d - 0.05\n", a, j);
            }
        }

    }

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
