#include "pbPlots.h"
#include "supportLib.h"

int tickCount=0;
//last 300ticks are averaged for output 1 and 2
float iterationAvg1=0;
float iterationAvg2=0;

//average output of 1 and 2 over 10 iterations
int avgOut1[10]={0,0,0,0,0,0,0,0,0,0};
int avgOutCount1=0;
float output1count=0;

int avgOut2[10]={0,0,0,0,0,0,0,0,0,0};
int avgOutCount2=0;
float output2count=0;

//overall network average over 10 iterations
int avg[10]={0,0,0,0,0,0,0,0,0,0};
int avgCount=0;
float avgAct=0;

//arrays for plotting neuron activity
double xss[2000];
double yss1[2000];
double yss2[2000];

void plot(double xs[], double ys1[], double ys2[], int len, char* name) {
	_Bool success;

	StartArenaAllocator();

    RGBABitmapImageReference *canvasReference1 = CreateRGBABitmapImageReference();

	StringReference *errorMessage = CreateStringReference(L"", 0);

    //output 1
    ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
	series->xs = xs;
	series->xsLength = len;
	series->ys = ys1;
	series->ysLength = len;
	series->linearInterpolation = true;
	series->pointType = L"solid";
	series->pointTypeLength = wcslen(series->pointType);
    series->lineThickness = 1;
	series->color = CreateRGBColor(0, 0, 1);

    //output 2
	ScatterPlotSeries *series2 = GetDefaultScatterPlotSeriesSettings();
	series2->xs = xs;
	series2->xsLength = len;
	series2->ys = ys2;
	series2->ysLength = len;
	series2->linearInterpolation = true;
	series2->lineType = L"solid";
	series2->lineTypeLength = wcslen(series->lineType);
	series2->lineThickness = 1;
	series2->color = CreateRGBColor(1, 0, 0);

	ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
	settings->width = 600;
	settings->height = 400;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
	settings->title = L"";
	settings->titleLength = wcslen(settings->title);
	settings->xLabel = L"";
	settings->xLabelLength = wcslen(settings->xLabel);
	settings->yLabel = L"";
	settings->yLabelLength = wcslen(settings->yLabel);
	ScatterPlotSeries *s [] = {series, series2};
	settings->scatterPlotSeries = s;
	settings->scatterPlotSeriesLength = 2;

	errorMessage = (StringReference *)malloc(sizeof(StringReference));
	success = DrawScatterPlotFromSettings(canvasReference1, settings, errorMessage);

	if(success){
		size_t length;
		ByteArray *pngdata = ConvertToPNG(canvasReference1->image);
		WriteToFile(pngdata, name);
		DeleteImage(canvasReference1->image);
	}else{
		fprintf(stderr, "Error: ");
		for(int i = 0; i < errorMessage->stringLength; i++){
			fprintf(stderr, "%c", errorMessage->string[i]);
		}
		fprintf(stderr, "\n");
	}

	FreeAllocations();
}

//average network activity
void activityAvg(int newFired) {
    if(avgCount<10) {
        avg[avgCount]=newFired;
        avgCount++;
    } else {
        for(int i=0;i<9;i++) {
            avg[i]=avg[i+1];
        }
        avg[9]=newFired;
    }
    int sum=0;
    for(int i=0;i<10;i++)
        sum+=avg[i];

    avgAct=sum/10.f;

    //plot output activity
    if(tickCount>=plotSize) {
        plot(xss, yss1, yss2, plotSize, "inter.png");
    } else{
        xss[tickCount]=tickCount;
        yss1[tickCount]=output1count;
        yss2[tickCount]=output2count;
    }
    tickCount++;
}

// TODO: take array as argument
void outputAvg(int first, int second) {
    if(avgOutCount1<10) {
        avgOut1[avgOutCount1]=first;
        avgOutCount1++;
    } else {
        for(int i=0;i<9;i++) {
            avgOut1[i]=avgOut1[i+1];
        }
        avgOut1[9]=first;
    }
    //2
    if(avgOutCount2<10) {
        avgOut2[avgOutCount2]=second;
        avgOutCount2++;
    } else {
        for(int i=0;i<9;i++) {
            avgOut2[i]=avgOut2[i+1];
        }
        avgOut2[9]=second;
    }



    output1count=0;
    output2count=0;

    for(int i=0;i<10;i++)
       output1count+=avgOut1[i];
    for(int i=0;i<10;i++)
       output2count+=avgOut2[i];

    output1count=output1count/10.f;
    output2count=output2count/10.f;
}


