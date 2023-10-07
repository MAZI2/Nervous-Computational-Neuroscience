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
float dpeak=1.7f;
float fatigue=0.3f;

float win[2][100];
float wrec[100][100];
float wout[100][2];

int restore=0;
int reward=1;

int trainNum=0;
int trainingNerves[1];

int seed=2;
