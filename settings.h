typedef struct _Nerve {
    float x, y;
    int id;
    float potential;
    float multiplier;
    int receptorType;
    int neuronType;
    float desensitize;
} Nerve;

static Nerve** inputs;
static Nerve** nerves;
static Nerve** outputs;

int inNum=2;
int outNum=2;
int recNum=100;
int recX=10;
int recY=10;

float threshold=17.f;
float dopamine=1.f;
float decay=0.3f; //0.05
float dpeak=1.7f;
float fatigue=0.5f;
float pathAdjust=0.1f;//0.1f;
int inputInterval=10;//3

float win[2][100];
float wrec[100][100];
float wout[100][2];

int restore=0;
int reward=1;

int trainNum=0;
int trainingNerves[1];

int seed=4;//4
int timer=0;//150000;
int inputEnable=1;
int enableStatus=0;
int trainedNerve=0;


int plotSize=1300;
int visualize=1;
int statistic=0;
int statisticInterval=10;


