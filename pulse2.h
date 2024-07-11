#include "connections.h"

int pulseTickCount=0;

void fireInputs() {
    for(int i=0;i<firedInputsNum;i++)
        inputs[i]->potential=22;
}

void periodicInputs() {
    if(pulseTickCount==5) {
        pulseTickCount=0;
        fireInputs();
    }
    pulseTickCount++;
}

void decreaseDopamine() {
    // dopamine decay    
    if(dopamine>1.f) {
         dopamine-=decay;
         if(dopamine<0) dopamine=0;
    }
}

void evaluateOutputs() {
    for(int i=0;i<OUT_NUM;i++) {
        if(firedOutputs[i] && correctOutputs[i]) {
            dopamine=dpeak; break;
        }
    }
}

void resetPotentials() {
    for(int i=0;i<REC_NUM;i++)
        if(previousFired[i])
            neurons[i]->potential=0;
    
    for(int i=0;i<IN_NUM;i++) 
         inputs[i]->potential=0;
    
    for(int i=0;i<OUT_NUM;i++) {
        if(outputs[i]->potential>threshold)
            outputs[i]->potential=0;
    }
}

void decreasePotentials() {
    for(int i=0;i<REC_NUM;i++) {
        if(!tempRecurrent[i])
            if(neurons[i]->potential-potentialDecay>0)
                neurons[i]->potential-=potentialDecay;
            else
                neurons[i]->potential=0;
    }
    for(int i=0;i<OUT_NUM;i++) {
        if(!tempOutputs[i]) 
            if(outputs[i]->potential-potentialDecay>0)
                outputs[i]->potential-=potentialDecay;
            else
                outputs[i]->potential=0;
    }
}

void sendPulse() {
    usleep(timer);

    for(int i=0;i<REC_NUM;i++) {
        if(!previousFired[i])
            continue;

        float neuronType=neurons[i]->neuronType;
        for(int j=0;j<REC_NUM;j++) {
            if(wrec[i][j]!=0) {
                float potential=neurons[j]->potential;
                float desensitize=neurons[j]->desensitize;
                int dReceptorType=neurons[j]->receptorType;
                // same neurons may be added multiple times but the temp array is only used for skipping
                tempRecurrent[j]=1;

                int previousPotential=potential;
                // excited by dopamine
                if(dReceptorType==1) {
                    potential+=(wrec[i][j]/(float)10*(float)55)*neuronType*dopamine*desensitize;

                // inhibited by dopamine
                } else {
                     potential+=(((wrec[i][j]/(float)10*(float)55)*neuronType)/dopamine)*desensitize;
                }

                if(potential<0) potential=0;

                if(potential>threshold && previousPotential<threshold)
                    currentFired[j]=1;

                neurons[j]->potential=potential;
            }
        }
    }
    // TODO: input neurons dont need to be separate. 
    // Just tag them and the code doesn't need to repeat the way it does
    
    // TODO: go only through firedInputs
    for(int i=0;i<IN_NUM;i++) {
        float neuronType=inputs[i]->neuronType;
        for(int j=0;j<REC_NUM;j++) {
            if(win[i][j]!=0 && inputs[i]->potential>threshold) {
                float potential=neurons[j]->potential;
                float desensitize=neurons[j]->desensitize;
                int dReceptorType=neurons[j]->receptorType;

                tempRecurrent[j]=1;
                
                int previousPotential=potential;
                
                if(dReceptorType==1) {
                    potential+=(win[i][j]/(float)10*(float)55)*neuronType*dopamine*desensitize;                    
                } else {
                    potential+=(((win[i][j]/(float)10*(float)55)*neuronType)/dopamine)*desensitize;
                }

                if(potential<0) potential=0;

                if(potential>threshold && previousPotential<threshold)
                    currentFired[j]=1;

                neurons[j]->potential=potential;
            }
        }
    }

    // TODO: move inside first loop
    for(int i=0;i<REC_NUM;i++) {
        if(!previousFired[i])
            continue;

        float neuronType=neurons[i]->neuronType;
        for(int j=0;j<OUT_NUM;j++) {
            float potential=outputs[j]->potential;
            float desensitize=outputs[j]->desensitize;
            //int dReceptorType=neurons[j]->receptorType;

            tempOutputs[j]=1;
            
            int previousPotential=potential;
            
            // commented to disable dopamine effect on output cells (don't want inhibited correct outputs ...)
            //if(dReceptorType==1) {
            
            potential+=(wout[i][j]/(float)10*(float)55)*neuronType*desensitize;

            //    potential+=(wout[i][j]/(float)10*(float)55)*neuronType*dopamine*desensitize;
            //} else {
            //    potential+=(((wout[i][j]/(float)10*(float)55)*neuronType)/dopamine)*desensitize;
            //}

            if(potential<0) potential=0;

            if(potential>threshold && previousPotential<threshold)
                firedOutputs[j]=1;

            outputs[j]->potential=potential;
        }
    }

    //TODO: everything below could be moved to main?

    // dopamine decay in every iteration
    decreaseDopamine();
    // dopamine reward
    evaluateOutputs();

    // reset fired potentials
    resetPotentials();
    // decrease potentials over time
    decreasePotentials();

    // adjust connections
    adjustConnections();
    // adjust fatigue
    adjustFatigue();

    // statistics
    updateOutputAvg();
    updateActivityAvg();

    //swap previousFired with current
    for(int i=0;i<REC_NUM;i++) {
        previousFired[i]=currentFired[i];
        currentFired[i]=0;
        tempRecurrent[i]=0;
    }
    for(int i=0;i<OUT_NUM;i++) {
        firedOutputs[i]=0;
        tempOutputs[i]=0;
    }

    // for periodic input firing
    periodicInputs();

    // recursive
    sendPulse();
}
