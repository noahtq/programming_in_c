//HW #16, Noah Turnquist

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAXFACTORIAL 20
#define EXITVAL 0
#define BITSINBYTE 8
#define UNSIGNED "unsigned"
#define FLUSH while(getchar() != '\n')

int GetNumFromUser(void);
unsigned long long Factorial(unsigned long long n);
void FindMaxFactorialByType(int sizeOfType, int isSigned);
void printBits(size_t const size, void const * const ptr); //TODO: Remove this function

int main(int argc, const char * argv[]) {
    int userNum = 1;
    unsigned long long result;
    
    printf("HW #16, Noah Turnquist\n");
    
    for (int i = 1; i < MAXFACTORIAL; i++) {
        printf("%d: %llu\n", i, Factorial(i));
    }
    
    //TODO: reenable this while loop once done debugging
//    while(userNum != 0) {
//        userNum = GetNumFromUser();
//        if (userNum > 0) {
//            result = Factorial((unsigned long long) userNum);
//            printf("The factorial of %d is %lld\n\n", userNum, result);
//        }
//    }
    
    putchar('\n');
    FindMaxFactorialByType(sizeof(int), 1);
    putchar('\n');
    FindMaxFactorialByType(sizeof(long long), 1);
    putchar('\n');
    FindMaxFactorialByType(sizeof(short), 1);
    putchar('\n');
    FindMaxFactorialByType(sizeof(int), 0);
    putchar('\n');
    FindMaxFactorialByType(sizeof(long long), 0);
    putchar('\n');
    FindMaxFactorialByType(sizeof(short), 0);
    
    return 0;
}

int GetNumFromUser(void) {
    int validSelection = 0;
    char strUserNum[4];
    int userNum = -1;
    
    do {
        putchar('\n');
        printf("Enter %d to exit.\n", EXITVAL);
        printf("Max Value - %d\n", MAXFACTORIAL);
        printf("Please enter a number: ");
        
        scanf(" %3s", strUserNum);
        int matched = (int) strspn(strUserNum, "0123456789");
        if (matched == strlen(strUserNum) && strlen(strUserNum) <= 2) {
            userNum = atoi(strUserNum);
            if (userNum >= EXITVAL && userNum <= MAXFACTORIAL) {
                validSelection = 1;
            } else {
                printf("Please enter a number between %d and %d.\n", EXITVAL, MAXFACTORIAL);
            }
        } else {
            printf("Please enter a valid number.\n");
        }
        FLUSH;
    } while (!validSelection);
    
    return userNum;
}

unsigned long long Factorial(unsigned long long n) {
    /*
     Use recursion to calculate a factorial
     */
    
    if (n <= 1) {
        return 1;
    }
    return n * Factorial(n - 1);
}

//TODO: Convert this function so it can also figure out max sizes for unsigned ints
//TODO: Currently having issue where I can't store UINT_MAX in unsigned long long.
void FindMaxFactorialByType(int sizeOfType, int isSigned) {
    int shifts;
    int n = 0;
    unsigned long long maxValue = 0x7FFFFFFFFFFFFFFF;
    unsigned long long maxFactorialValue = 0;
    unsigned long long prevFactorialValue = 0;
    char unsignedStr[strlen(UNSIGNED) + 1] = "";
    
//    if (isSigned) {
//        maxValue = ;
//    } else {
//        maxValue =
//    }
    
//    printBits(sizeof(maxSignedLong), &maxSignedLong);
    shifts = (sizeof(maxValue) - sizeOfType) * BITSINBYTE;
    
//    if (isSigned) shifts--;
    
    maxValue = maxValue >> shifts;
    
    if (!isSigned) {
        maxValue *= 2;
        strncpy(unsignedStr, UNSIGNED, strlen(UNSIGNED));
    } 
    
    int i, foundMax;
    for (i = 1, foundMax = 0; i <= MAXFACTORIAL + 1 && !foundMax; i++) {
        maxFactorialValue = Factorial(i);
        if (maxFactorialValue >= maxValue) {
            maxFactorialValue = prevFactorialValue;
            foundMax = 1;
            n = i - 1;
        }
        prevFactorialValue = maxFactorialValue;
    }
    
    printf("For an %s integer of size %d bytes,\n", unsignedStr, sizeOfType);
    printf("the max factorial value is %lld which is given by %d!\n", maxFactorialValue, n);
}

//TODO: Remove this function
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}
