#include "connections.h"

int inp=0;
int phase=0;
int counter=0;
int timer=100000;


int avgOut1[10]={0,0,0,0,0,0,0,0,0,0};
int avgOutCount1=0;
float output1count=0;

int avgOut2[10]={0,0,0,0,0,0,0,0,0,0};
int avgOutCount2=0;
float output2count=0;

int avg[10]={0,0,0,0,0,0,0,0,0,0};
int avgCount=0;
float avgAct=0;

void activityAvg(int newFired) {
    if(avgCount<10) {
        avg[avgCount]=newFired;
        avgCount++;
    } else {
        for(int i=0;i<9;i++) {
            avg[i]=avg[i+1];
        }
        avg[9]=newFired;
    }
    int sum=0;
    for(int i=0;i<10;i++)
        sum+=avg[i];

    avgAct=sum/10.f;
}
void outputAvg(int first, int second) {
    if(avgOutCount1<10) {
        avgOut1[avgOutCount1]=first;
        avgOutCount1++;
    } else {
        for(int i=0;i<9;i++) {
            avgOut1[i]=avgOut1[i+1];
        }
        avgOut1[9]=first;
    }
    //2
    if(avgOutCount2<10) {
        avgOut2[avgOutCount2]=second;
        avgOutCount2++;
    } else {
        for(int i=0;i<9;i++) {
            avgOut2[i]=avgOut2[i+1];
        }
        avgOut2[9]=second;
    }



    output1count=0;
    output2count=0;

    for(int i=0;i<10;i++)
       output1count+=avgOut1[i];
    for(int i=0;i<10;i++)
       output2count+=avgOut2[i];

    output1count=output1count/10.f;
    output2count=output2count/10.f;
}

void sendPulse() { //Nerve** activeNerves, int* activeNum) {
   tempFired=0;

   usleep(timer);

   Nerve* temp[100];
   int anyActivity=0;

   //incremented
   int count=0;

   //reccurent nerves iteration
   for(int i=0;i<recNum;i++) {
       float potential=nerves[i]->potential;
       float desensitize=nerves[i]->desensitize;
       int receptorType=nerves[i]->receptorType;
       //iterate through connections and check if there is a path and sufficient potential to fire

       //was current neuron incremented
       int add=0;
       for(int j=0;j<recNum;j++) {
           //inhibitory/excicatory 
           float multiplier=nerves[j]->multiplier;
           //skip connection if it was incremented in this iteration
           int inThis=0;            
           for(int k=0;k<count;k++) {
                if(nerves[j]==temp[k]) {
                    inThis=1;
                    break;
                }
           }

           if(wrec[j][i]!=0 && nerves[j]->potential>threshold && !inThis && potential<threshold) {
               add=1;

               if(receptorType==1) 
                    potential+=(wrec[j][i]/(float)10*(float)55)*multiplier*dopamine*desensitize; 
               else
                    potential+=((wrec[j][i]/(float)10*(float)55)*multiplier)/dopamine*desensitize;
               if(potential<0) potential=0;

               //printf("R %d -> %d (%f) %f\n", j, i, wrec[j][i]/(float)10*(float)55, potential);

               //neurons are still firing
               anyActivity=1;
               
               //add to fired
               if(potential>threshold) {
                    tempPrev[tempFired]=i;
                    tempFired++;
               }
           }
       }

       //same for inputs
       for(int j=0;j<inNum;j++) {
           float multiplier=inputs[j]->multiplier;
           
           if(win[j][i]!=0 && inputs[j]->potential>threshold) {
               add=1;

               if(receptorType==1) 
                    potential+=(win[j][i]/(float)10*(float)55)*multiplier*dopamine*desensitize;
               else
                    potential+=((win[j][i]/(float)10*(float)55)*multiplier)/dopamine*desensitize;

               if(potential<0) potential=0;

               //printf("I %d -> %d (%f) %f\n", j, i, win[j][i]/(float)10*(float)55, potential);
               anyActivity=1;

               //add to fired
               if(potential>threshold) {
                    tempPrev[tempFired]=i;
                    tempFired++;
               }

           }
       }


       //add neuron to temp if it received any input
       if(add) {
           //printf("%d\n", i);
           temp[count]=nerves[i];
           count++;
       }

       nerves[i]->potential=potential;
   }

   //set output to 0 if it fired
   for(int i=0;i<outNum;i++) {
        if(outputs[i]->potential>threshold)
            outputs[i]->potential=0;
   }

   int outputActivity=0; 
   int wrong=0;

   //increment output neurons
   for(int i=0;i<outNum;i++) {
       float desensitize=outputs[i]->desensitize;
       for(int j=0;j<recNum;j++) {
           float multiplier=nerves[j]->multiplier;

           if(wout[j][i]>0 && nerves[j]->potential>threshold) {
               outputs[i]->potential+=wout[j][i]/(float)10*(float)55*multiplier*desensitize; 

           if(outputs[i]->potential<0) outputs[i]->potential=0;

               
           } 
       }
        if(outputs[0]->potential>threshold) {
            outputActivity=1;
        }
        if(outputs[1]->potential>threshold) {
            wrong=1;
        }

   }

   if(outputActivity && wrong)
       outputAvg(1, 1);
   else if(outputActivity)
       outputAvg(1, 0);
   else if(wrong)
       outputAvg(0, 1);
   else
       outputAvg(0, 0);
   //dopamine and second phase
   if(outputActivity==1) {
       //    printf("reached\n");
       if(reward)
            dopamine=dpeak;

       if(counter>60)  {
           phase=1;
           timer=100000;
       }

       //printf("COUNTER %d\n", counter);
       counter++;
   }
   if(wrong==1) {
       /*
       if(reward)
            dopamine=dpeak;

       if(counter>60)  {
           phase=1;
           timer=100000;
       }

       //printf("COUNTER %d\n", counter);
       counter++;

       */
       //printf("wrong\n");
       //not sure about this
       /* if(reward && counter<50) */
       /*     if(dopamine>1.f) */
       /*         dopamine=1.f; */
       /*     else */
       /*         dopamine=1/dpeak; */
   } else if(dopamine>1.f) {
        dopamine=0.f;
        //dopamine-=0.05f;
        //printf("D: %f\n", dopamine);
   } else if(dopamine<1.f) {
        dopamine=0.f;
        //dopamine+=0.05f;
        //printf("D: %f\n", dopamine);
   }

   //set inputs to 0 after firing
   for(int i=0;i<inNum;i++) {
       if(inputs[i]->potential>threshold)
           inputs[i]->potential=0;
            

//        printf("%d: %f\n", i, inputs[i]->potential);               
   
   }

   if(phase==0) {
       if(inp==3) {
           inputs[1]->potential=21;

           /*
           for(int i=0;i<trainNum;i++) {
                //purpose of thread args ... ??
                int a=trainingNerves[i];

                nerves[a]->potential=21; 
                temp[count]=nerves[a]; 

                tempPrev[tempFired]=a; 

                tempFired++; 
                count++;
           }
           */
           inp=0;
           anyActivity=1;
           counter++;
       } else {
           anyActivity=1;
           inp++;
       }
   } else {
       if(inp==250) {
           inputs[1]->potential=21;
            inp=0;
            anyActivity=1;
       } else {
            inp++;
            anyActivity=1;
       }
   }

   //set neurons not in temp if >threshold to 0
   for(int i=0;i<recNum;i++) {
        int skip=0;
        for(int j=0;j<recNum;j++) {
            if(nerves[i]==temp[j]) {
                skip=1;
                break;
            }
        }

        if(!skip && nerves[i]->potential>threshold)
            nerves[i]->potential=0;

        //deacrease over time if not in temp
        if(!skip) {
            if(nerves[i]->potential-0.3f>0)
                nerves[i]->potential-=0.3f;
            else
                nerves[i]->potential=0;
        }

//        printf("%d: %f\n", i, nerves[i]->potential);               
   }

//   printf("----\n");

   if(anyActivity) {
       activityAvg(tempFired);
       //printf("%f\n", act);
       adjustConnections();
        sendPulse();
   } else {
        int reset=1;
        for(int i=0;i<recNum;i++) {
            if(nerves[i]->potential>0) {
                reset=0;
                break;
            }
        } 
        //if all neurons 0 reset inputs
        if(reset) {
            for(int i=0;i<outNum;i++) {
                if(outputs[i]->potential>0) {
                    reset=0;
                    break;
                }
            }

            //make array of inputs global!
            if(reset) {
                int inputArr[2]={1, 0};
                for(int i=0;i<inNum;i++) {
                   if(inputArr[i]) inputs[i]->potential=21; 
                }    
                sendPulse();
            }
        }
        
   }
}
