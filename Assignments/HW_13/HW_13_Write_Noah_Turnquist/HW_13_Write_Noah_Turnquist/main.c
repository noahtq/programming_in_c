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

enum FILEOPTIONS { APPEND = 1, OVERWRITE, RENAME, ABORT };

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
    
    fPt = OpenWrBinaryFile(fullFilePath, fileLength);
    printf("%s\n", fullFilePath);
    
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
    } while(fileExists || fileOption == RENAME);
    
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
        printf("\tAppend to the file (%d),\n", APPEND);
        printf("\tOverwrite the file (%d),\n", OVERWRITE);
        printf("\tChange the filename (%d),\n", RENAME);
        printf("\tOr abort (%d)\n", ABORT);
        printf("Enter a number: ");
        if (scanf(" %d", &fileOption) == 1) {
            if (fileOption >= APPEND && fileOption <= ABORT) {
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
