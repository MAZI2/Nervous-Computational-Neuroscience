int firedInputs[4]; 
int numFired;

int expectedOutput[4]={1, 1, 0, 0};

void outputPicture(int* array) {
   for(int i=0;i<4;i++) {
       if(outputs[i]->potential>20) array[i]=1;
   } 
   return;
}

void evaluate() {
   int sequential=0;

   float error=0;
   int* array=calloc(4, sizeof(int));
   outputPicture(array);

   for(int i=0;i<4;i++) {
       if(!sequential) {
           int active=0;
           printf("%d %d :%d\n", expectedOutput[i], array[i], (expectedOutput[i]-array[i])*(expectedOutput[i]-array[i]));
           error+=(expectedOutput[i]-array[i])*(expectedOutput[i]-array[i]);
       }     
   } 
   error/=4;
   printf("error: %f\n", error);
}
