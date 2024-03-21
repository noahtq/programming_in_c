//HW #10, Noah Turnquist

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int GetNumSets(void);
int GetNumStartEndPts(void);
int GetNumPts(void);
int* CreateDataset(int);
void SortArray(int*, int);
double Average(int*, int);

int main(void) {
    
    srand((unsigned int) time(NULL));
    
    int nSets = rand() % 6 + 5;
    int* dataSet[nSets + 1];
    dataSet[nSets] = NULL;
    int c = 0;
    int* p = c;
    dataSet[3] = p;
    
    printf("NSets: %d\n", nSets);
    int** itr = dataSet;
    while (*itr != NULL) {
        printf("Hello\n");
        itr++;
    }
    
    printf("HW #10, Noah Turnquist\n");
    
    return 0;
}
