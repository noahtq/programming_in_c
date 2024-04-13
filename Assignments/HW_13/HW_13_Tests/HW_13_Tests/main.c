//
//  main.c
//  HW_13_Tests
//
//  Created by Noah Turnquist on 4/13/24.
//

#include <stdio.h>
#include "hw_13_write.h"

#define MAXFILENAMESIZE 20

void PrintTestResults(int testSuccess, int expectedSuccess);
int TestGetFileNameFromUser(void);

int main(int argc, const char * argv[]) {
    
    
    return 0;
}

void PrintTestResults(int testSuccess, int expectedSuccess) {
    printf("Ran %d tests.\n", expectedSuccess);
    if (testSuccess == expectedSuccess) {
        printf("All tests passed.\n");
    } else {
        printf("Tests passed: %d, tests failed: %d\n", testSuccess, expectedSuccess - testSuccess);
    }
}

int TestGetFileNameFromUser(void) {
    char filename[MAXFILENAMESIZE + 1];
    
    GetFileNameFromUser(filename)
}
