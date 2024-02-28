#include "connections.h"

//counter for inputInterval between inputs
int inp=0;
//general purpose counter ... currently counts input fires
int counter=0;


//extremely long pulse function
void sendPulse() {
   // number of neurons that reached the threshold this iteration (stored in tempPrev[])
   tempFired=0;

   usleep(timer);

   // neurons that received input in this iteration
   // TODO: use macro
   Nerve* temp[100];

   // any activity in this iteration
   // int anyActivity=0;

   //incremented
   int count=0;

   //reccurent nerves iteration (inputs from recurrent and input -> to recurrent)
   // TODO: should probbably be incremented the other way round 
   // TODO: (check each active neuron and increment it's neighbours ... adjacency list for better time complexity)
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
   // -------------------------------------

   // TODO: move following to separate function
   
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

   // TODO: any output array size
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

       
       // TODO: should do any size of output array
       if(outputs[0]->potential>threshold) {
           //storing fired outputs to properly adjust their weights
           outputsFired[0]=1;
       } else
           outputsFired[0]=0;

       if(outputs[1]->potential>threshold) {
           outputsFired[1]=1;
       } else
           outputsFired[1]=0;
   }
   //decrease outputs (do not decrease if it was incremented in this iteration)
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


   //dopamine ... for every output that is rewarded, increase dopamine ... need to make general
   // TODO: move to a separate function
   if(outputsFired[0]==1 && trainedNerve==0) {
       if(reward)
            dopamine=dpeak;
   }
   if(outputsFired[1]==1 && trainedNerve==1) {
       if(reward)
            dopamine=dpeak;
   } else if(dopamine>1.f) {
        dopamine-=decay;
   } else if(dopamine<1.f) {
        dopamine+=decay;
   }

   // TODO: move to a separate function
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
   // TODO: should take array as argument
   // TODO: could slow down the system if many outputs
   // TODO: outputAvg(firedOutputs, len)
   if(outputsFired[0] && outputsFired[1])
       outputAvg(1, 1);
   else if(outputsFired[0])
       outputAvg(1, 0);
   else if(outputsFired[1])
       outputAvg(0, 1);
   else
       outputAvg(0, 0);


   //average overall activity 
   activityAvg(tempFired);

   //increase and decrease connections
   adjustConnections();

   // TODO: move to a separate function
   if(tickCount<=plotSize) {
       //if 1300 ticks ... after 1000th start recording outputs
       if(tickCount>1000 && statistic) {
           iterationAvg1+=output1count;
           iterationAvg2+=output2count;
       }

       //FOR INPUT AS A SEQUENCE
       // INPUT FUNCTION
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
       // END OF ITERATION
   } else {
       //average outputs last 300ticks ... are plotted
       iterationAvg1/=300;
       iterationAvg2/=300;

       pthread_exit(NULL);
   }
}
