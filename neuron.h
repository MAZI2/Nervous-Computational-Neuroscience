typedef struct _Nerve {
    float x, y;
    int id;
    float potential;
    //excitatory 1 / inhibitory -1
    int neuronType;
    //response to dopamine excitatoryu 1 / inhibitory -1
    int receptorType;
    //current fatigue
    float desensitize;
} Nerve;

static Nerve** inputs;
static Nerve** neurons;
static Nerve** outputs;

//boolean arrays i-th element = i-th neuron

//array of fired neurons from previous iteration
int previousFired[REC_NUM];
//array of fired in current iteration
int currentFired[REC_NUM];
//array of inputs that received any potential in current iteration
int tempRecurrent[REC_NUM];

//array of fired outputs in current iteration
int firedOutputs[OUT_NUM];
//array of rewarded outputs (built from trainedNeurons[] in settings)
int correctOutputs[OUT_NUM];
//array of outputs that received any potential in current iteration
int tempOutputs[OUT_NUM];

