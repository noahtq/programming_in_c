// HW #03, Noah Turnquist

#include <stdio.h>

int GetInteger(int *pNum);
int Get2FloatingValues(float *pfVal, double *pdVal);
void PrintValues(int iNum, float fVal, double dVal);

int main(void) {
    int num;
    float flt;
    double dub;
    int intsRead;
    int floatsRead;
    
    printf("HW #03, Noah Turnquist\n");
    
    intsRead = GetInteger(&num);
    floatsRead = Get2FloatingValues(&flt, &dub);
    
    printf("The user entered %d integer and %d floating point numbers.\n", intsRead, floatsRead);
    PrintValues(num, flt, dub);
    
    return 0;
}

int GetInteger(int *pNum) {
    printf("Please enter one integer value: ");
    return scanf("%d", pNum);
}

int Get2FloatingValues(float *pfVal, double *pdVal) {
    printf("Please enter two floating point numbers separated by a space: ");
    return scanf("%f %lf", pfVal, pdVal);
}

void PrintValues(int iNum, float fVal, double dVal) {
    printf("The user entered: %d, %g, %g\n", iNum, fVal, dVal);
}


//HW #03, Noah Turnquist
//Please enter one integer value: 567892
//Please enter two floating point numbers separated by a space: 0.0000000987 6.76e10
//The user entered 1 integer and 2 floating point numbers.
//The user entered: 567892, 9.87e-08, 6.76e+10
//Program ended with exit code: 0
