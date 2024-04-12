//HW #13, Noah Turnquist

//TODO: Add documentation for all functions
//TODO: Ask Professor about append option. It is in his sample output but not in the program description.

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

enum FILEOPTIONS { OVERWRITE = 1, RENAME, ABORT };

int GetNumPoints(void);
double GetRandDouble(void);
FILE* OpenWrBinaryFile(char* fullFileName, int fullFilePathSize);
void GetFileNameFromUser(char* fileName);
void AppendFileNameToFilePath(const char* fileName, const char* filepath, char* fullPath, int stringLength);
int CheckFileExists(const char* filepath);
int GetUserFileOption(const char* fileName);

int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));
    
    FILE* fPt;
    int fileLength = strlen(FILEPATH) + MAXFILENAMESIZE;
    char fullFilePath[fileLength + 1];
    
    if ((fPt = OpenWrBinaryFile(fullFilePath, fileLength)) != NULL) {
        printf("%s\n", fullFilePath);
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

FILE* OpenWrBinaryFile(char* fullFileName, int fullFilePathSize) {
    char fileName[MAXFILENAMESIZE + 1];
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
 */
