//number of inputs, outputs, recurrent
#define IN_NUM 2
#define OUT_NUM 2
#define REC_NUM 400
//dimensions of recurrent
#define REC_X 20
#define REC_Y 20

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
//potential decay over time
float potentialDecay=0.3f;
//rate of resensitizing
float resensitizeRate=0.1f;
//how much connections are adjusted
float pathAdjust=0.1f;//0.1f;
//time between input fires
int inputInterval=2;//3

float win[IN_NUM][REC_NUM];
float wrec[REC_NUM][REC_NUM];
float wout[REC_NUM][OUT_NUM];

//restore saved connections
int restore=0;
//enable dopamine
int reward=1;

//for association training
/*
int trainNum=0;
int trainingNerves[1];
*/

int seed=2;//4
//delay between two iterations
int timer=100000;
//enable input
int inputEnable=1;
//show status
int enableStatus=1;
//rewarded output
int trainedNeurons[]={1};
int trainedNeuronsNum=1;
//input to be fired
int firedInputs[]={1};
int firedInputsNum=1;
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
