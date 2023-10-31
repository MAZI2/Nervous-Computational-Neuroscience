typedef struct _Nerve {
    float x, y;
    int id;
    float potential;
    //excitatory 1 / inhibitory -1
    //same as neuron type
    float multiplier;
    int neuronType;
    //response to dopamine excitatoryu 1 / inhibitory -1
    int receptorType;
    //current fatigue
    float desensitize;
} Nerve;

static Nerve** inputs;
static Nerve** nerves;
static Nerve** outputs;

//number of inputs, outputs, recurrent
int inNum=2;
int outNum=2;
int recNum=100;
//dimensions of recurrent
int recX=10;
int recY=10;

//action potential treshold
float threshold=18.f;
//base dopamine level
float dopamine=1.f;
//dopamine decay over time
float decay=0.15f; //0.05
//dopamine spike if correct output
float dpeak=2.f;//1.7f;
//neuron fatigue(cooldown) after firing ... fatigue decay is 0.1
float fatigue=0.5f;
//how much connections are adjusted
float pathAdjust=0.1f;//0.1f;
//time between input fires
int inputInterval=2;//3

float win[2][100];
float wrec[100][100];
float wout[100][2];

//restore saved connections
int restore=0;
//enable dopamine
int reward=1;

//for association training
/*
int trainNum=0;
int trainingNerves[1];
*/

int seed=39;//4
//delay between two iterations
int timer=100000;
//enable input
int inputEnable=1;
//show status
int enableStatus=1;
//rewarded output
int trainedNerve=1;
//input to be fired
int inputNerve=1;

//input sequence
/*
int inputNerves[150]={1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0};

//{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//{1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1};
*/

//number of ticks to be plotted
int plotSize=1300;//1700;//1300;

//show neuron visualization
int visualize=1;
//statistics mode - traverse statisticInterval amout of seeds and plot averages
int statistic=0;
int statisticInterval=50;

//find and artificially strengthen I->O path
int pretrained=0;
