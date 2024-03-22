//HW #10, Noah Turnquist

//TODO: ADD FUNCTION SORTING METHOD WITH SELECTION SORT

//TODO: Ask professor if I need verification code for this assignment
//TODO: update function declerations to have parameter names matching definitions
//TODO: Add documentation describing what each function does
//TODO: Rewrite while statements looping through pointers to be  for loops looping through pointers

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define OUTPUT_STR_SIZE 29
#define HEADER_STR_SIZE 61

int GetNumSets(void);
int GetNumStartEndPts(void);
int GetNumPts(void);
double* CreateDataset(int arrSize);
void SortArray(double* arr, int arrSize);
double Average(double* arr, int arrSize);
void CreateDisplayStr(char* str, int* nPtsToPrint, int atStart);

int main(void) {
    
    srand((unsigned int) time(NULL));
    
    int nSets = GetNumSets();
    int nPtsToPrint = GetNumStartEndPts();
    double* dataSet[nSets + 1];
    dataSet[nSets] = NULL;
    int ptsPerRow[nSets]; //TODO: Get rid of this array if possible. See if there is a better way to get the number of pts in a row of the ragged array and pass to functions like average() without this

    //TODO: Update array to use pointer notation
    for (int i = 0; i < nSets; i++) {
        int nPts = GetNumPts();
        ptsPerRow[i] = nPts;
        dataSet[i] = CreateDataset(nPts);
    }
    
    printf("HW #10, Noah Turnquist\n\n");
    
    //TODO: Put all of this into a function
    char headerStr[HEADER_STR_SIZE];
    
    switch(nPtsToPrint) {
        case 2:
            strncpy(headerStr, "First Data Pts     Last Data Pts", HEADER_STR_SIZE - 1);
            break;
        case 3:
            strncpy(headerStr, " ___First Data Pts___     ___Last Data Pts____", HEADER_STR_SIZE - 1);
            break;
        case 4:
            strncpy(headerStr, " ______First Data Pts_______     _______Last Data Pts_______", HEADER_STR_SIZE - 1);
            break;
        default:
            strncpy(headerStr, "Error: something went wrong.", HEADER_STR_SIZE - 1);
    }
    
    printf("Set #  Npts  Average %s\n", headerStr);
    
    //TODO: split this loop into some seperate functions
    double** itr = dataSet;
    int* ptsPerRowP = ptsPerRow;
    while (*itr != NULL) {
        SortArray(*itr, *ptsPerRow);
//        for (int i = 0; i < *ptsPerRow; i++) { //TODO: Remove this statement. Just for debug.
//            printf("%lf\n", (*itr)[i]);
//        }
        
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
        
        double* end = *itr + *ptsPerRow;
        double* arr = *itr;
        switch(nPtsToPrint) {
            case 2:
                sprintf(startPts, formatStr, *arr, *(arr + 1));
                sprintf(endPts, formatStr, *(end - 2), *(end - 1));
                break;
            case 3:
                sprintf(startPts, formatStr, *arr, *(arr + 1), *(arr + 2));
                sprintf(endPts, formatStr, *(end - 3), *(end - 2), *(end - 1));
                break;
            case 4:
                sprintf(startPts, formatStr, *arr, *(arr + 1), *(arr + 2), *(arr + 3));
                sprintf(endPts, formatStr, *(end - 4), *(end - 3), *(end - 2), *(end - 1));
                break;
            default:
                sprintf(startPts, "Error. No data.");
                sprintf(endPts, "Error. No data.");
        }
        
        printf("  %d   %5d   %2.4lf  %s ... %s\n", (int) (itr - dataSet), *ptsPerRowP, average, startPts, endPts);
        
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

void SortArray(double* arr, int arrSize) {
    for (double* arrPoint = arr; arrPoint < arr + arrSize; arrPoint++) {
        double* min = arrPoint;
        for (double* arrNestedPoint = arrPoint; arrNestedPoint < arr + arrSize; arrNestedPoint++) {
            if (*arrNestedPoint < *min) {
                min = arrNestedPoint;
            }
        }
        double temp = *arrPoint;
        *arrPoint = *min;
        *min = temp;
    }
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
