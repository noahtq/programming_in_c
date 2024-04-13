//HW #13, Noah Turnquist - Read

//TODO: Add default functionality
//TODO: Add documentation to functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILEPATH "/Users/noahturnquist/Documents/College/Spring_2024/Programming_in_C/Assignments/HW_13/"
#define MAXFILENAMESIZE 20
#define FILEEXTENSION ".xxx"
#define FLUSH while(getchar() != '\n')

typedef struct {
    int datasetNum;
    int nPts;
    double average;
    double points[];
} DATA;

FILE* OpenReadBinaryFile(char* fullFileName, int fullFilePathSize, char* fileName);
void GetFileNameFromUser(char* fileName);
void AppendFileNameToFilePath(const char* fileName, const char* filepath, char* fullPath, int stringLength);
DATA* ReadInDataFromFile(FILE* fPtr);
int CompareAverages(double x, double y);
double CalculateAverage(DATA* dataPtr);
void PrintOutput(DATA* dataPtr, char* fileName, int averagesMatch, double calculatedAverage);

int main(int argc, const char * argv[]) {
    FILE* fPtr;
    int fileLength = strlen(FILEPATH) + MAXFILENAMESIZE;
    char fileName[MAXFILENAMESIZE + 1];
    char fullFilePath[fileLength + 1];
    DATA* dataPtr;
    double calculatedAverage;
    int averagesMatch;
    
    printf("HW #13 Part 2: Noah Turnquist\n");
    
    if((fPtr = OpenReadBinaryFile(fullFilePath, fileLength, fileName)) == NULL) {
        printf("Couldn't open binary file.\n");
        return 1;
    }
    
    if ((dataPtr = ReadInDataFromFile(fPtr)) == NULL) {
        printf("Error retrieving data from file.\n");
        return 1;
    }
    
    calculatedAverage = CalculateAverage(dataPtr);
    averagesMatch = CompareAverages(dataPtr->average, calculatedAverage);
    
    printf("%s\n", fullFilePath);
    PrintOutput(dataPtr, fileName, averagesMatch, calculatedAverage);
    
    
    fclose(fPtr);
    free(dataPtr);
    dataPtr = NULL;
    
    return 0;
}

FILE* OpenReadBinaryFile(char* fullFileName, int fullFilePathSize, char* fileName) {
    //TODO: Add ability to leave file if wanted
    
    FILE* fPtr;
    
    do {
        GetFileNameFromUser(fileName);
        AppendFileNameToFilePath(fileName, FILEPATH, fullFileName, fullFilePathSize);
        if ((fPtr = fopen(fullFileName, "rb")) == NULL) {
            printf("Couldn't find file. Try again.\n");
        }
    } while(fPtr == NULL);
    
    return fPtr;
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

DATA* ReadInDataFromFile(FILE* fPtr) {
    int bytesOffset;
    int dataSize;
    
    //Read in struct size integer in bytes from file that is before each struct
    //Offset file pointer to point past the first struct
    fread(&bytesOffset, sizeof(int), 1, fPtr); //TODO: Add error checking
    fseek(fPtr, bytesOffset, SEEK_CUR);
    
    //Read in the size of the second struct in bytes so we can allocate space
    //for it on the heap
    fread(&dataSize, sizeof(int), 1, fPtr); //TODO: Add error checking
    DATA* dataPtr = (DATA*) malloc(dataSize);
    if (dataPtr == NULL) {
        printf("Couldn't allocate space on heap for DATA struct.\n");
        return NULL;
    }
    fread(dataPtr, dataSize, 1, fPtr); //TODO: Add error checking
    return dataPtr;
}

int CompareAverages(double x, double y) {
    return x == y ? 1 : 0;
}

double CalculateAverage(DATA* dataPtr) {
    double sum = 0.0;
    for (int i = 0; i < dataPtr->nPts; i++) {
        sum += dataPtr->points[i];
    }
    return sum / dataPtr->nPts;
}

void PrintOutput(DATA* dataPtr, char* fileName, int averagesMatch, double calculatedAverage) {
    printf("Read from file: %s\n", fileName);
    printf("Dataset #%d: %5dbytes\n", dataPtr->datasetNum, (int) (sizeof(DATA) + sizeof(double) * dataPtr->nPts));
    printf("Number of data points: %d\n", dataPtr->nPts);
    printf("Average from file: %4.4lf\n", dataPtr->average);
    printf("Average calculated: %4.4lf\n", calculatedAverage);
    if (averagesMatch) {
        printf("The averages are equal.\n");
    } else {
        printf("The averages don't match.\n");
    }
}
