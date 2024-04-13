//
//  hw_13_write.h
//  HW_13_Tests
//
//  Created by Noah Turnquist on 4/13/24.
//

#ifndef hw_13_write_h
#define hw_13_write_h

#include <stdio.h>

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

#endif /* hw_13_write_h */
