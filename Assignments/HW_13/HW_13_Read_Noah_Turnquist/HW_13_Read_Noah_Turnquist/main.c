//HW #13, Noah Turnquist - Read

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULTFILENAME "default"
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
    int fileLength = strlen(FILEPATH) + MAXFILENAMESIZE + strlen(FILEEXTENSION);
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
    /*
     Attempt to open a file in read binary mode and return a pointer to that
     file stream.
     User is prompted for filename and given several options if file already
     exists. There is also an option to use a default filename.
     */
    
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
    /*
     Read in a filename from the user or use default filename if no name
     is provided.
     */
    
    printf("The default file name is: %s\n", DEFAULTFILENAME);
    printf("Enter a file name up to %d chars: ", MAXFILENAMESIZE);
    fgets(fileName, MAXFILENAMESIZE, stdin);
    if (fileName[0] == '\n') {
        strncpy(fileName, DEFAULTFILENAME, MAXFILENAMESIZE);
    }
    else if (!strchr(fileName, '\n')) {
        FLUSH;
    } else {
        *strchr(fileName, '\n') = '\0';
    }
}

void AppendFileNameToFilePath(const char* fileName, const char* filepath, char* fullPath, int stringLength) {
    /*
     Add together a filename, filepath, and extension.
     Ensure that total filepath size does not exceed stringLength
     */
    
    //Copy the file path. After copy subtract the characters written from
    //stringLength
    strncpy(fullPath, filepath, stringLength);
    stringLength -= strlen(filepath);
    
    //Concatenate file name to fullPath. After Concatination subtract the length
    //of fileName from stringLength.
    strncat(fullPath, fileName, stringLength);
    stringLength -= strlen(fileName);
    
    //Add the file extension.
    strncat(fullPath, FILEEXTENSION, stringLength);
}

DATA* ReadInDataFromFile(FILE* fPtr) {
    /*
     Read in the second DATA structure contained in the file pointed to by fPtr.
     Store read in DATA struct on the heap and return a pointer to said struct.
     If second DATA struct can't be read in return NULL.
     */
    
    int bytesOffset;
    int dataSize;
    
    //Read in struct size integer in bytes from file that is before each struct
    //Offset file pointer to point past the first struct
    if(!fread(&bytesOffset, sizeof(int), 1, fPtr)) {
        printf("Error reading size of first struct.\n");
        return NULL;
    }
    fseek(fPtr, bytesOffset, SEEK_CUR);
    
    //Read in the size of the second struct in bytes so we can allocate space
    //for it on the heap
    if(!fread(&dataSize, sizeof(int), 1, fPtr)) {
        printf("Error reading size of second struct.\n");
        return NULL;
    }
    DATA* dataPtr = (DATA*) malloc(dataSize);
    if (dataPtr == NULL) {
        printf("Couldn't allocate space on heap for DATA struct.\n");
        return NULL;
    }
    if (!fread(dataPtr, dataSize, 1, fPtr)) {
        printf("Error reading in struct from file.\n");
        return NULL;
    }
    return dataPtr;
}

int CompareAverages(double x, double y) {
    /*
     Return true if x equals y, false otherwise.
     */
    
    return x == y ? 1 : 0;
}

double CalculateAverage(DATA* dataPtr) {
    /*
     Loop through all points in DATA struct pointed to by dataPtr.
     Return the average.
     */
    
    double sum = 0.0;
    for (int i = 0; i < dataPtr->nPts; i++) {
        sum += dataPtr->points[i];
    }
    return sum / dataPtr->nPts;
}

void PrintOutput(DATA* dataPtr, char* fileName, int averagesMatch, double calculatedAverage) {
    /*
     Output the filename and dataset information from the DATA struct read in
     from the file.
     Output whether the averages match or not.
     */
    
    printf("Read from file: %s\n", fileName);
    printf("Dataset #%d: %5d bytes\n", dataPtr->datasetNum, (int) (sizeof(DATA) + sizeof(double) * dataPtr->nPts));
    printf("Number of data points: %d\n", dataPtr->nPts);
    printf("Average from file: %4.4lf\n", dataPtr->average);
    printf("Average calculated: %4.4lf\n", calculatedAverage);
    if (averagesMatch) {
        printf("The averages are equal.\n");
    } else {
        printf("The averages don't match.\n");
    }
}

//HW #13 Part 2: Noah Turnquist
//The default file name is: default
//Enter a file name up to 20 chars: HW13Data
//Users/noahturnquist/Documents/College/Spring_2024/Programming_in_C/Assignments/HW_13/HW13Data.xxx
//Read from file: HW13Data
//Dataset #2:  8016 bytes
//Number of data points: 1000
//Average from file: 10.5316
//Average calculated: 10.5316
//The averages are equal.
//Program ended with exit code: 0

/*
 TEST PLAN
 
 TEST OPENREADBINARYFILE()
 
 1. Enter filename that doesn't exist: HW01Data
 -Should print can't find file message and ask you to try again
 
 2. (Default behavior)
    Leave blank and hit nothing
 -Should set the default filename to default
 
 3. Enter filename that does exist: HW13Data
 -Should prepend full filepath and add file extension to end of filename
 -Should open file in read binary mode
 -Return pointer to file stream
 
 TEST REST OF PROGRAM
 1. In HW_13_Write program, write out HW13Data.xxx
    Open this program (read program) and enter HW13Data
    Compare values of the second DATA struct in each program.
 -Size of struct should match
 -Dataset number should match
 -Number of data points should match
 -Averages should be equal
 
 2. Add breakpoint at bottom of main, just before return statement
 -File stream should be closed
 -Memory on heap should be deallocated and dataPtr set to NULL
 */
