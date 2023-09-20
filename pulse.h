#include "connections.h"

int inp=0;
int phase=0;
int counter=0;
int timer=100000;

void sendPulse() { //Nerve** activeNerves, int* activeNum) {
   tempFired=0;

   usleep(timer);

   Nerve* temp[40];
   int anyActivity=0;

   //incremented
   int count=0;

   //reccurent nerves iteration
   for(int i=0;i<recNum;i++) {
       float potential=nerves[i]->potential;
       float multiplier=nerves[i]->multiplier;
       //iterate through connections and check if there is a path and sufficient potential to fire

       //was current neuron incremented
       int add=0;
       for(int j=0;j<recNum;j++) {
           //skip connection if it was incremented in this iteration
           int inThis=0;            
           for(int k=0;k<count;k++) {
                if(nerves[j]==temp[k]) {
                    inThis=1;
                    break;
                }
           }

           if(wrec[j][i]!=0 && nerves[j]->potential>20 && !inThis && potential<20) {
               add=1;
               potential+=(wrec[j][i]/(float)10*(float)55)*multiplier; 
               if(potential<0) potential=0;

               //printf("R %d -> %d (%f) %f\n", j, i, wrec[j][i]/(float)10*(float)55, potential);

               //neurons are still firing
               anyActivity=1;
               
               //add to fired
               if(potential>20) {
                    tempPrev[tempFired]=i;
                    tempFired++;
               }
           }
       }

       //same for inputs
       for(int j=0;j<inNum;j++) {
           if(win[j][i]!=0 && inputs[j]->potential>20) {
               add=1;

               potential+=(win[j][i]/(float)10*(float)55)*multiplier;
               if(potential<0) potential=0;

               //printf("I %d -> %d (%f) %f\n", j, i, win[j][i]/(float)10*(float)55, potential);
               anyActivity=1;

               //add to fired
               if(potential>20) {
                    tempPrev[tempFired]=i;
                    tempFired++;
               }

           }
       }


       //add neuron to temp if it received any input
       if(add) {
           temp[count]=nerves[i];
           count++;
       }

       nerves[i]->potential=potential;
   }

   //set output to 0 if it fired
   for(int i=0;i<outNum;i++) {
        if(outputs[i]->potential>20)
            outputs[i]->potential=0;
   }

   int outputActivity=0; 

   //increment output neurons
   for(int i=0;i<outNum;i++) {
       for(int j=0;j<recNum;j++) {
           if(wout[j][i]>0 && nerves[j]->potential>20) {
               if(i==0)
                   outputActivity=1;

               outputs[i]->potential+=wout[j][i]/(float)10*(float)55; 
           } 
       }
   }

   //dopamine and second phase
   if(outputActivity==1) {
       if(counter>100)  {
           phase=1;
           timer=100000;
       }

       printf("COUNTER %d\n", counter);
       counter++;
   }

   //set inputs to 0 after firing
   for(int i=0;i<inNum;i++) {
       if(inputs[i]->potential>20)
           inputs[i]->potential=0;
            

//        printf("%d: %f\n", i, inputs[i]->potential);               
   
   }

   if(phase==0) {
       if(inp==1) {
            inputs[0]->potential=21;
            nerves[4]->potential=21;
            temp[count]=nerves[4];
            tempPrev[tempFired]=4;
            tempFired++;
            count++;
            inp=0;
            anyActivity=1;
       } else {
            inp=1;
       }
   } else {
       nerves[14]->multiplier=1.5f;
       if(inp==3) {
           inputs[0]->potential=21;
            inp=0;
            anyActivity=1;
       } else {
            inp++;
            anyActivity=1;
       }
   }

   //set neurons not in temp if >20 to 0
   for(int i=0;i<recNum;i++) {
        int skip=0;
        for(int j=0;j<recNum;j++) {
            if(nerves[i]==temp[j]) {
                skip=1;
                break;
            }
        }

        if(!skip && nerves[i]->potential>20)
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

   printf("----\n");

   if(anyActivity) {
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
