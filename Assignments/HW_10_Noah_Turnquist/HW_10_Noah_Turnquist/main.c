//HW #10, Noah Turnquist

//TODO: ADD FUNCTION SORTING METHOD WITH SELECTION SORT

//TODO: Ask professor if I need verification code for this assignment
//TODO: update function declerations to have parameter names matching definitions
//TODO: Add documentation describing what each function does

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define OUTPUT_STR_SIZE 22

int GetNumSets(void);
int GetNumStartEndPts(void);
int GetNumPts(void);
double* CreateDataset(int arrSize);
void SortArray(int*, int);
double Average(double* arr, int arrSize);
void CreateDisplayStr(char* str, int* nPtsToPrint, int atStart);

int main(void) {
    
    srand((unsigned int) time(NULL));
    
    for(int i = 0; i < 1000; i++) {
        printf("%d\n", GetNumStartEndPts());
    }
    
    int nSets = GetNumSets();
    int nPtsToPrint = GetNumStartEndPts();
    double* dataSet[nSets + 1];
    dataSet[nSets] = NULL;
    int ptsPerRow[nSets]; //TODO: Get rid of this array if possible. See if there is a better way to get the number of pts in a row of the ragged array and pass to functions like average() without this
    
    //TODO: Ask professor if using this notation to get array started is alright? I'm starting this way because I had issues where the pointers in the ragged array would randomly be set to NULL values before they're initialized ending the loop prematurely when I attempted to loop by detecting NULL pointers.
    for (int i = 0; i < nSets; i++) {
        int nPts = GetNumPts();
        ptsPerRow[i] = nPts;
        dataSet[i] = CreateDataset(nPts);
    }
    
    printf("HW #10, Noah Turnquist\n\n");
    printf("Set #  Npts  Average  ___First Data Pts___     ___Last Data Pts____\n");
    
    double** itr = dataSet;
    int* ptsPerRowP = ptsPerRow;
    while (*itr != NULL) {
        double average = Average(*itr, *ptsPerRowP);
        char formatStr[OUTPUT_STR_SIZE] = "";
        char startPts[OUTPUT_STR_SIZE] = "";
        char endPts[OUTPUT_STR_SIZE] = "";
        
//        TODO: Update this to be more efficient
        //TODO: Update this to be able to accomidate up to 4 pts
        for (int i = 0; i < nPtsToPrint; i++) {
            strncat(formatStr, "%1.4lf", 6);
            if (i < nPtsToPrint - 1) {
                strncat(formatStr, " ", 1);
            }
        }
        
//        printf("NPtstoPrint: %d, FormatStr: %s\n", nPtsToPrint, formatStr); //TODO: Remove this
        double* end = *itr + *ptsPerRow;
        double* arr = *itr;
        switch(nPtsToPrint) {
            case 1:
                sprintf(startPts, formatStr, *arr);
                sprintf(endPts, formatStr, *end);
                break;
            case 2:
                sprintf(startPts, formatStr, *arr, *(arr + 1));
                sprintf(endPts, formatStr, *(end - 1), *end);
                break;
            case 3:
                sprintf(startPts, formatStr, *arr, *(arr + 1), *(arr + 2));
                sprintf(endPts, formatStr, *(end - 2), *(end - 1), *end);
                break;
            default:
                sprintf(startPts, "Error. No data.");
                sprintf(endPts, "Error. No data.");
        }
        
        printf("  %d   %5d   %2.4lf %21s ... %s\n", (int) (itr - dataSet), *ptsPerRowP, average, startPts, endPts);
        
        free(*itr);
        *itr = NULL;
        itr++;
        ptsPerRowP++;
    }
    
    printf("\n*** Bye ***\n");
    
    return 0;
}

int GetNumSets(void) {
    return rand() % 6 + 5;
}

int GetNumStartEndPts(void) {
    return rand() % 3 + 2;
}

int GetNumPts(void) {
    return 1000 * (rand() % 8 + 4);
}

double* CreateDataset(int arrSize) {
    double* pArr = (double*) malloc(arrSize * sizeof(double));
    if (pArr == NULL) {
        printf("Couldn't allocate memory on the heap.\n");
        return NULL;
    }
    
    double* end = pArr + arrSize;
    double* itr = pArr;
    while (itr < end) {
        double randDouble = rand() * 10.0 / RAND_MAX;
        *itr = randDouble;
        itr++;
    }
    
    return pArr;
}

double Average(double* arr, int arrSize) {
    double* end = arr + arrSize;
    double* itr = arr;
    double sum = 0.0;
    while (itr < end) {
        sum += *itr;
        itr++;
    }
    return sum / arrSize;
}

void CreateDisplayStr(char* str, int* nPtsToPrint, int atStart) {
    /*
     Create the display string for the output.
     Populate with nPtsToPrint double values from array.
     Take the first few values or the last few values depending on boolean value of atStart.
     */
}
