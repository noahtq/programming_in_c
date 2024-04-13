//
//  hw_13_write.c
//  HW_13_Tests
//
//  Created by Noah Turnquist on 4/13/24.
//

#include "hw_13_write.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILEPATH "/Users/noahturnquist/Documents/College/Spring_2024/Programming_in_C/Assignments/HW_13/"
#define MAXFILENAMESIZE 20
#define FILEEXTENSION ".xxx"
#define NUMVLAS 2
#define FLUSH while(getchar() != '\n')

int GetNumPoints(void) {
    return 200 * (rand() % 5 + 1);
}

double GetRandDouble(void) {
    return 20.0 * rand() / RAND_MAX;
}

FILE* OpenWrBinaryFile(char* fullFileName, int fullFilePathSize, char* fileName) {
    int fileOption = -1;
    int fileExists;
    FILE* fPt;
    
    do {
        GetFileNameFromUser(fileName);
        AppendFileNameToFilePath(fileName, FILEPATH, fullFileName, fullFilePathSize);
        if ((fileExists = CheckFileExists(fullFileName)) == 1) {
            fileOption = GetUserFileOption(fileName);
        }
    } while(fileExists && fileOption == RENAME);
    
    if (fileExists == 0 || fileOption == OVERWRITE) {
        fPt = fopen(fullFileName, "wb");
    } else {
        fPt = NULL;
    }
    return fPt;
}

void GetFileNameFromUser(char* fileName) {
    printf("Enter a file name up to %d chars: ", MAXFILENAMESIZE);
    scanf("%20s", fileName); //TODO: Get rid of magic constant in this scanf
    if (!strchr(fileName, '\n')) {
        FLUSH;
    }
}

void AppendFileNameToFilePath(const char* fileName, const char* filepath, char* fullPath, int stringLength) {
    strncpy(fullPath, filepath, stringLength);
    stringLength -= strlen(filepath);
    
    strncat(fullPath, fileName, stringLength);
    stringLength -= strlen(fileName);
    
    strncat(fullPath, FILEEXTENSION, stringLength);
}

int CheckFileExists(const char* filepath) {
    FILE* fCheck;
    int fileExists;
    if ((fCheck = fopen(filepath, "r")) != NULL) {
        fileExists = 1;
    } else {
        fileExists = 0;
    }
    fclose(fCheck);
    return fileExists;
}

int GetUserFileOption(const char* fileName) {
    //TODO: Might need to add FLUSH to this function
    int fileOption;
    int validEntry = 0;
    
    do {
        printf("File %s exists. Do you want to:\n", fileName);
        printf("\tOverwrite the file (%d),\n", OVERWRITE);
        printf("\tChange the filename (%d),\n", RENAME);
        printf("\tOr abort (%d)\n", ABORT);
        printf("Enter a number: ");
        if (scanf(" %d", &fileOption) == 1) {
            if (fileOption >= OVERWRITE && fileOption <= ABORT) {
                validEntry = 1;
            } else {
                printf("Please enter a valid number from the list of options.\n");
            }
        } else {
            printf("Please enter a number.\n");
        }
        FLUSH;
    } while (validEntry == 0);
    
    return fileOption;
}

int CreateVLAStructsOnHeap(DATA* dataArr[]) {
    int structsMade = 0;
    
    for (int i = 0; i < NUMVLAS; i++) {
        int numPts = GetNumPoints();
        DATA* dataPtr = (DATA*) malloc(sizeof(DATA) + sizeof(double) * numPts);
        if (dataPtr == NULL) {
            dataArr[i] = NULL;
            printf("Couldn't allocate memory on heap for DATA struct.\n");
        } else {
            dataPtr->datasetNum = i + 1;
            dataPtr->nPts = numPts;
            double sum = 0.0;
            for (int j = 0; j < numPts; j++) {
                double tempDouble = GetRandDouble();
                sum += tempDouble;
                dataPtr->points[j] = tempDouble;
            }
            dataPtr->average = sum / numPts;
            dataArr[i] = dataPtr;
            structsMade++;
        }
    }
    return structsMade;
}

int WriteVLASStructsToFile(FILE* fPtr, DATA* dataArr[]) {
    int structsWritten = 0;
    
    for (int i = 0; i < NUMVLAS; i++) {
        int structSize = sizeof(DATA) + (dataArr[i])->nPts * sizeof(double);
        //Add size of structure in bytes before each struct in file
        fwrite(&structSize, sizeof(int), 1, fPtr); //TODO: Maybe remove this? Add error checking if I keep it.
        if (fwrite(dataArr[i], structSize, 1, fPtr)) {
            structsWritten++;
        }
    }
    
    return structsWritten;
}

void PrintDataInfo(DATA* dataArr[], char* fileName) {
    for (int i = 0; i < NUMVLAS; i++) {
        printf("Wrote dataSet #%d to %s: %5d bytes\n", (dataArr[i])->datasetNum, fileName, (int) (sizeof(DATA) + sizeof(double) * (dataArr[i])->nPts));
        printf("The set contains %5d points, the average is %4.4lf\n", (dataArr[i])->nPts, (dataArr[i])->average);
    }
}

void DeallocateMemoryFromHeap(DATA* dataArr[]) {
    for (int i = 0; i < NUMVLAS; i++) {
        free(dataArr[i]);
        dataArr[i] = NULL;
    }
}
