typedef struct _Nerve {
    float x, y;
    int id;
    float potential;
    float multiplier;
} Nerve;

static Nerve** inputs;
static Nerve** nerves;
static Nerve** outputs;

int inNum=2;
int outNum=2;
int recNum=40;

float win[2][40];
float wrec[40][40];
float wout[40][2];

