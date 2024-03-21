//
//  main.c
//  inclass_practice_01
//
//  Created by Noah Turnquist on 3/14/24.
//

//Print values out on one line using array notation
//Print values out on one line using pointer notation
//Print values out on one line by incrementing a pointer

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 6

double Random(void);
void PrintWithArrayNotation(double []);
void PrintWithPointerNotation(double []);
void PrintByIncrementingPointer(double []);

int main(void) {
    srand(time (NULL));
    printf("Output from traditional array notation loop\n");
    double dArr1[SIZE];
    int i;
    for(i = 0; i < SIZE; i++) {
        dArr1[i] = Random();
    }
    
    PrintWithArrayNotation(dArr1);
    PrintByIncrementingPointer(dArr1);
    PrintWithPointerNotation(dArr1);
    
    return 0;
}

double Random(void) {
    return rand() * 10.0 / RAND_MAX;
}

void PrintWithArrayNotation(double arr[]) {
    int n;
    for (int i = 0, n = 0; i < SIZE; i++) {
        printf("%g ", arr[i]);
        if (++n == 3) {
            putchar('\n');
            n = 0;
        }
    }
    putchar('\n');
}

void PrintWithPointerNotation(double arr[]) {
    double *pArr = (double*) malloc(SIZE * sizeof(double));
    if (pArr) {
        for (int i = 0; i < SIZE; i++) {
            *pArr = arr[i];
            pArr++;
        }
        
        pArr -= 6;
        
        int n;
        for (int i = 0, n = 0; i < SIZE; i++) {
            printf("%g ", *pArr++);
            if (++n == 3) {
                putchar('\n');
                n = 0;
            }
        }
        putchar('\n');
        
        free(pArr);
        pArr = NULL;
    } else {
        printf("Failed to allocate memory\n");
    }
}

void PrintByIncrementingPointer(double arr[]) {
    int n;
    for (int i = 0, n = 0; i < SIZE; i++) {
        printf("%g ", *(arr++));
        if (++n == 3) {
            putchar('\n');
            n = 0;
        }
    }
    putchar('\n');
}
