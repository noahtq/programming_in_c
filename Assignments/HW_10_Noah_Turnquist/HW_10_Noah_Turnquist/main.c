//HW #10, Noah Turnquist

//TODO: Ask professor if I need verification code for this assignment
//TODO: Ask professor if there is a better way to get nPts for each loop iteration in the second main loop. Currently creating a second array to store that info.

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
void PrintHeaderStr(int nPts);
void PrintDisplayStrs(int nPtsToPrint, int index, int nPts, double average, double* arr);

int main(void) {
    
    srand((unsigned int) time(NULL));
    
    int nSets = GetNumSets();
    int nPtsToPrint = GetNumStartEndPts();
    double* dataSet[nSets + 1];
    dataSet[nSets] = NULL;
    int ptsPerRow[nSets]; //TODO: Get rid of this array if possible. See if there is a better way to get the number of pts in a row of the ragged array and pass to functions like average() without this

    int* ptsPerRowItr = ptsPerRow;
    for (double** itr = dataSet; itr < dataSet + nSets; itr++, ptsPerRowItr++) {
        int nPts = GetNumPts();
        *ptsPerRowItr = nPts;
        *itr = CreateDataset(nPts);
    }
    
    printf("HW #10, Noah Turnquist\n\n");
    
    PrintHeaderStr(nPtsToPrint);
    
    double** itr = dataSet;
    int* ptsPerRowP = ptsPerRow;
    while (*itr != NULL) {
        SortArray(*itr, *ptsPerRow);
        
        double average = Average(*itr, *ptsPerRowP);
        
        PrintDisplayStrs(nPtsToPrint, (int) (itr - dataSet), *ptsPerRowP, average, *itr);
        
        free(*itr);
        *itr = NULL;
        itr++;
        ptsPerRowP++;
    }
    
    printf("\n*** Bye ***\n");
    
    return 0;
}

int GetNumSets(void) {
    //Return a random integer between 5 and 10. [5, 10]
    return rand() % 6 + 5;
}

int GetNumStartEndPts(void) {
    //Return a random integer between 2 and 4. [2, 4]
    return rand() % 3 + 2;
}

int GetNumPts(void) {
    /*
     Return a random number between 4,000 and 11,000. [4000, 11000]
     Number will be an even thousand i.e. 4000, 5000, 6000, etc.
     */
    return 1000 * (rand() % 8 + 4);
}

double* CreateDataset(int arrSize) {
    /*
     Create an array on the heap of arrSize elements
     Return a pointer to that array
     */
    
    double* pArr = (double*) malloc(arrSize * sizeof(double));
    if (pArr == NULL) {
        printf("Couldn't allocate memory on the heap.\n");
        return NULL;
    }
    
    for (double* itr = pArr; itr < pArr + arrSize; itr++) {
        double randDouble = rand() * 10.0 / RAND_MAX;
        *itr = randDouble;
    }
    
    return pArr;
}

void SortArray(double* arr, int arrSize) {
    /*
     Sort an array using pointer notation.
     Sorting algorithm is Selection Sort - Time Complexity: O(n^2) i.e. Quadratic Time
     */
    
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
    /*
     Calculate the average of an array of data.
     The array is allocated on the heap and is pointed to by arr.
     Return the average.
     */
    
    double sum = 0.0;
    for (double* itr = arr; itr < arr + arrSize; itr++) {
        sum += *itr;
        itr++;
    }
    return sum / arrSize;
}

void PrintHeaderStr(int nPts) {
    /*
     Create and print a header string for the data being output.
     String is adjusted based on nPts.
     */
    
    char headerStr[HEADER_STR_SIZE];
    
    switch(nPts) {
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
}

void PrintDisplayStrs(int nPtsToPrint, int index, int nPts, double average, double* arr) {
    /*
     Create and print the output string.
     Output string will be formatted correctly depending on the number in nPtsToPrint.
     */
    
    char formatStr[OUTPUT_STR_SIZE] = "";
    char startPts[OUTPUT_STR_SIZE] = "";
    char endPts[OUTPUT_STR_SIZE] = "";
    
    //Create format string with correct number of type identifiers based
    //on nPtsToPrint
    for (int i = 0; i < nPtsToPrint; i++) {
        strncat(formatStr, "%1.4lf", 6);
        if (i < nPtsToPrint - 1) {
            strncat(formatStr, " ", 1);
        }
    }
    
    double* end = arr + nPts;
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
    
    printf("  %d   %5d   %2.4lf  %s ... %s\n", index, nPts, average, startPts, endPts);
}

//HW #10, Noah Turnquist
//
//Set #  Npts  Average  ______First Data Pts_______     _______Last Data Pts_______
//  0    7000   2.5167  0.0009 0.0021 0.0021 0.0031 ... 9.9969 9.9970 9.9984 9.9996
//  1   10000   2.4927  0.0017 0.0034 0.0051 0.0066 ... 3.7433 2.8537 1.9155 3.5142
//  2    7000   2.4696  0.0006 0.0009 0.0035 0.0068 ... 9.9919 9.9959 9.9968 9.9976
//  3    8000   2.5021  0.0000 0.0000 0.0010 0.0036 ... 5.0794 9.9049 1.1231 5.6703
//  4    4000   1.3825  0.0000 0.0000 0.0000 0.0000 ... 5.7318 5.7363 5.7363 5.7399
//  5    4000   0.5092  0.0000 0.0000 0.0000 0.0000 ... 6.2617 6.2619 6.2637 6.2657
//  6   10000   2.4739  0.0024 0.0081 0.0082 0.0097 ... 1.7768 2.1832 2.6563 5.1036
//  7    9000   2.5033  0.0000 0.0014 0.0030 0.0035 ... 0.4333 1.7067 4.6531 4.5501
//  8    5000   0.9096  0.0000 0.0000 0.0000 0.0000 ... 6.0367 6.0484 6.0498 6.0600
//  9    8000   2.4965  0.0008 0.0028 0.0034 0.0049 ... 5.7550 4.8983 5.4954 0.4928
//
//*** Bye ***
//Program ended with exit code: 0
