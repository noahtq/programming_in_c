//HW #10, Noah Turnquist

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
    int ptsPerRow[nSets];
    
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
        double average = Average(*itr, *ptsPerRowP);
        SortArray(*itr, *ptsPerRowP);
        
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
    
    double* end = arr + arrSize;
    for (double* arrPoint = arr; arrPoint < end; arrPoint++) {
        double* min = arrPoint;
        for (double* arrNestedPoint = arrPoint; arrNestedPoint < end; arrNestedPoint++) {
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
    
    double sum;
    sum = 0.0;
    for (double* itr = arr; itr < arr + arrSize; itr++) {
        sum += *itr;
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
    double* end = arr + nPts;
    
    //Create format string with correct number of type identifiers based
    //on nPtsToPrint
    for (int i = 0; i < nPtsToPrint; i++) {
        strncat(formatStr, "%1.4lf", 6);
        if (i < nPtsToPrint - 1) {
            strncat(formatStr, " ", 1);
        }
    }
    
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
//Set #  Npts  Average  ___First Data Pts___     ___Last Data Pts____
//  0    9000   5.0074  0.0005 0.0028 0.0035 ... 9.9921 9.9979 9.9983
//  1   10000   4.9980  0.0010 0.0023 0.0029 ... 9.9969 9.9995 9.9997
//  2    5000   4.9981  0.0033 0.0049 0.0055 ... 9.9942 9.9979 9.9980
//  3    9000   4.9840  0.0010 0.0018 0.0035 ... 9.9984 9.9992 9.9993
//  4    9000   5.0385  0.0009 0.0013 0.0030 ... 9.9964 9.9983 9.9995
//  5    8000   5.0061  0.0010 0.0036 0.0044 ... 9.9970 9.9985 9.9998
//  6   11000   4.9776  0.0008 0.0036 0.0065 ... 9.9995 9.9996 9.9999
//  7    8000   5.0027  0.0001 0.0020 0.0067 ... 9.9969 9.9980 9.9990
//
//*** Bye ***
//Program ended with exit code: 0
