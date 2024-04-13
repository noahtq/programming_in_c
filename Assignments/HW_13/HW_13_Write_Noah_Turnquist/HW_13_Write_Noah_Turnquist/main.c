//HW #13, Noah Turnquist - Write

//TODO: Add documentation for all functions
//TODO: Update sample output

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEFAULTFILENAME "default"
#define FILEPATH "/Users/noahturnquist/Documents/College/Spring_2024/Programming_in_C/Assignments/HW_13/"
#define MAXFILENAMESIZE 20
#define FILEEXTENSION ".xxx"
#define NUMVLAS 2
#define FLUSH while(getchar() != '\n')

typedef struct {
    int datasetNum;
    int nPts;
    double average;
    double points[];
} DATA;

enum FILEOPTIONS { OVERWRITE = 1, RENAME, ABORT };

int GetNumPoints(void);
double GetRandDouble(void);
FILE* OpenWrBinaryFile(char* fullFileName, int fullFilePathSize, char* fileName);
void GetFileNameFromUser(char* fileName);
void AppendFileNameToFilePath(const char* fileName, const char* filepath, char* fullPath, int stringLength);
int CheckFileExists(const char* filepath);
int GetUserFileOption(const char* fileName);
int CreateVLAStructsOnHeap(DATA* dataArr[]); //TODO: Consider switching to pointer notation for array of pointers parameter.
int WriteVLASStructsToFile(FILE* fPtr, DATA* dataArr[]); //TODO: Consider switching to pointer notation for array of pointers parameter.
void PrintDataInfo(DATA* dataArr[], char* fileName);
void DeallocateMemoryFromHeap(DATA* dataArr[]);

int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));
    
    FILE* fPt;
    int fileLength = strlen(FILEPATH) + MAXFILENAMESIZE + strlen(FILEEXTENSION);
    char fileName[MAXFILENAMESIZE + 1];
    char fullFilePath[fileLength + 1];
    DATA* dataArr[NUMVLAS];
    
    printf("HW #13 Part 1: Noah Turnquist\n");
    
    if (CreateVLAStructsOnHeap(dataArr) != NUMVLAS) {
        printf("Something went wrong while creating VLA Structs on the heap.\n");
        return 1;
    }
    
    if ((fPt = OpenWrBinaryFile(fullFilePath, fileLength, fileName)) != NULL) {
        if (WriteVLASStructsToFile(fPt, dataArr) != NUMVLAS) {
            printf("Something went wrong writing VLA Structs to file.\n");
            return 1;
        }
        printf("%s\n", fullFilePath);
        putchar('\n');
        PrintDataInfo(dataArr, fileName);
        DeallocateMemoryFromHeap(dataArr);
    }
    
    fclose(fPt);
    
    return 0;
}

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
        putchar('\n');
    }
}

void DeallocateMemoryFromHeap(DATA* dataArr[]) {
    for (int i = 0; i < NUMVLAS; i++) {
        free(dataArr[i]);
        dataArr[i] = NULL;
    }
}


//HW #13 Part 1: Noah Turnquist
//Enter a file name up to 20 chars: HW13Data
//File HW13Data exists. Do you want to:
//    Overwrite the file (1),
//    Change the filename (2),
//    Or abort (3)
//Enter a number: 1
//
//Wrote dataSet #1 to HW13Data:  3216 bytes
//The set contains   400 points, the average is 10.4018
//
//Wrote dataSet #2 to HW13Data:  8016 bytes
//The set contains  1000 points, the average is 9.9373
//
//Program ended with exit code: 0


/*
 TEST PLAN
 
 CHECK OPENWRBINARYFILE()
 
 1. Enter filename that does not exist: HW20Data
 -Should exit function immediately with pointer to opened file
 
 2. Enter filename that does exist: HW13Data
    In next prompt choose option 1: Overwrite
 -Should exit function immediately with pointer to opened file, HW13Data
 
 3. Enter filename that does exist: HW13Data
    In next prompt choose option 2: Rename
    Enter a new filename when prompted: HW21Data
 -Should restart loop and ask user to enter filename
 -Once new filename is entered...
 -Should exit function immediately with pointer to opened file
 
 4. Enter filename that does exist: HW13Data
    In next prompt choose option 3: Abort
 -Should exit function immediately with a NULL pointer
 
 5. Enter a filename that is too long: HW13DataHW13DataHW13Data
 -Should cut off any chars beyond char 20 and continue program execution as normal
 
 6. Enter filename that does exist: HW13Data
    In next prompt enter non digit: abc
 -Should print "Please enter a number"
 -Prompt is repeated until valid number is entered
 
 7. Enter filename that does exist: HW13Data
    In next prompt enter too low digit: 0
 -Should print "Please enter a valid number from the list of options."
 -Prompt is repeated until valid number is entered
 
 8. Enter filename that does exist: HW13Data
    In next prompt enter too high digit: 9
 -Should print "Please enter a valid number from the list of options."
 -Prompt is repeated until valid number is entered
 
 9. (Testing buffer is flushed)
    Enter filename that does exist: HW13Data
    In next prompt enter invalid digit with valid digit: 9 2
 -Should print "Please enter a valid number from the list of options."
 -Prompt is repeated until valid number is entered
 
 10. Check default filename: Hit enter with no input
 -Should open a file named default if default hasn't already been created
 -Should prompt for file option if default already exists
 
 
 CHECK REST OF PROGRAM
 
 1. Should have two datasets created at expected file location: HW13Data
 2. Each dataset should have two different sizes in bytes, somewhere between 1,000 to 10,000
 3. Each dataset should output somewhere between 200 and 1000 points
 4. Average should be somewhere around 10.0 as the min double value is 0 and the max is 20
 5. Add breakpoint just before return in main to ensure that files are closed and memory is deallocated
 */
