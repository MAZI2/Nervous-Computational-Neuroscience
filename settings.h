typedef struct _Nerve {
    float x, y;
    int id;
    float potential;
    float multiplier;
    int receptorType;
    int neuronType;
} Nerve;

static Nerve** inputs;
static Nerve** nerves;
static Nerve** outputs;

int inNum=2;
int outNum=2;
int recNum=100;
int recX=10;
int recY=10;

float threshold=15.f;
float dopamine=1.f;

float win[2][100];
float wrec[100][100];
float wout[100][2];

int restore=0;
int reward=1;

int trainNum=5;
int trainingNerves[5];
