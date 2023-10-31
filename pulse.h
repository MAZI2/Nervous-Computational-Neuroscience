#include "connections.h"

//to wait inputInterval between input fires 
int inp=0;
//general purpose counter ... currently counts input fires
int counter=0;


//extremely long pulse function
void sendPulse() {
   tempFired=0;

   usleep(timer);

   Nerve* temp[100];
   int anyActivity=0;

   //incremented
   int count=0;

   //reccurent nerves iteration (inputs from recurrent and input -> to recurrent)
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

               if(receptorType==1)  {
                   potential+=(wrec[j][i]/(float)10*(float)55)*multiplier*dopamine*desensitize; 
               } else {
                   potential+=((wrec[j][i]/(float)10*(float)55)*multiplier)/dopamine*desensitize;
               }



               if(potential<0) potential=0;

               //printf("R %d -> %d (%f) %f\n", j, i, wrec[j][i]/(float)10*(float)55, potential);

               //neurons are still firing
               //anyActivity=1;
               
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
                   
               if(receptorType==1)  {
                    potential+=(win[j][i]/(float)10*(float)55)*multiplier*dopamine*desensitize;
               } else 
                    potential+=((win[j][i]/(float)10*(float)55)*multiplier)/dopamine*desensitize;

               if(potential<0) potential=0;

               //printf("I %d -> %d (%f) %f\n", j, i, win[j][i]/(float)10*(float)55, potential);
               //anyActivity=1;

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
   //end of iteration -------------------------------------
   
   //set inputs to 0 after firing
   for(int i=0;i<inNum;i++) {
       if(inputs[i]->potential>threshold)
           inputs[i]->potential=0;
           inputsFired[i]=0;
   }

   //set output to 0 if fired
   for(int i=0;i<outNum;i++) {
       if(outputs[i]->potential>threshold)
           outputs[i]->potential=0;
   }

   int outputActivity=0; 
   int outputActivity1=0;

   int outSkips[2]={0, 0};

   //increment output neurons
   for(int i=0;i<outNum;i++) {
       float desensitize=outputs[i]->desensitize;
       for(int j=0;j<recNum;j++) {
           float multiplier=nerves[j]->multiplier;

           if(wout[j][i]>0 && nerves[j]->potential>threshold) {
               outputs[i]->potential+=wout[j][i]/(float)10*(float)55*multiplier*desensitize; 

               if(outputs[i]->potential<0) 
                   outputs[i]->potential=0;

               outSkips[i]=1;
           } 
       }

       
       if(outputs[0]->potential>threshold) {
           outputActivity=1;
           outputsFired[0]=1;
       } else
           outputsFired[0]=0;

       if(outputs[1]->potential>threshold) {
           outputActivity1=1;
           outputsFired[1]=1;
       } else
           outputsFired[1]=0;
   }
   //decrease outputs
   for(int i=0;i<outNum;i++) {
        if(!outSkips[i]) {
           if(outputs[i]->potential-0.3f>0)
               outputs[i]->potential-=0.3f;
           else
               outputs[i]->potential=0;
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
   }


   //dopamine
   if(outputActivity==1 && trainedNerve==0) {
       if(reward)
            dopamine=dpeak;
   }

   if(outputActivity1==1 && trainedNerve==1) {
       if(reward)
            dopamine=dpeak;
   } else if(dopamine>1.f) {
        dopamine-=decay;
   } else if(dopamine<1.f) {
        dopamine+=decay;
   }

   if(inputEnable) {
       if(inp==inputInterval) {
           //SINGLE INPUT
           inputs[inputNerve]->potential=50;
           inputsFired[inputNerve]=1;

           //FIRE CERTAIN NON-INPUT NERVES (for association)
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
           counter++;
       } else {
           inp++;
       }
   }

//  --------------------------------------- 

   //a dumb way of recording activity of output 1 and 2
   if(outputActivity && outputActivity1)
       outputAvg(1, 1);
   else if(outputActivity)
       outputAvg(1, 0);
   else if(outputActivity1)
       outputAvg(0, 1);
   else
       outputAvg(0, 0);


   //average overall activity
   activityAvg(tempFired);

   //increase and decrease connections
   adjustConnections();

   if(tickCount<=plotSize) {
       //if 1300 ticks ... after 1000th start recording outputs
       if(tickCount>1000 && statistic) {
           iterationAvg1+=output1count;
           iterationAvg2+=output2count;
       }

       //FOR INPUT AS A SEQUENCE
       /*
       //if simulation goes beyond the input sequence, repeat last input
       if(counter>149) {
            //inputNerve=inputNerves[149];
            inputNerve=1;
            //trainedNerve=inputNerves[149];
            trainedNerve=0;
       } else {
             inputNerve=inputNerves[counter];
             if(counter<40)
                 trainedNerve=1;
             else
                 trainedNerve=1;
       }
       */

       //SINGLE REPEATED INPUT
       inputNerve=1;
       trainedNerve=1;

       sendPulse();
   } else {
       //average outputs last 300ticks ... are plotted
       iterationAvg1/=300;
       iterationAvg2/=300;

       pthread_exit(NULL);
   }
}
