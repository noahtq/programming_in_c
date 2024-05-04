//HW #16, Noah Turnquist

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXFACTORIAL 20
#define EXITVAL 0
#define BITSINBYTE 8
#define UNSIGNED "unsigned "
#define FLUSH while(getchar() != '\n')

int GetNumFromUser(void);
unsigned long long Factorial(unsigned long long n);
void FindMaxFactorialByType(int sizeOfType, int isSigned);

int main(int argc, const char * argv[]) {
    int userNum = 1;
    unsigned long long result;
    
    printf("HW #16, Noah Turnquist\n");
    
    while(userNum != 0) {
        userNum = GetNumFromUser();
        if (userNum > 0) {
            result = Factorial((unsigned long long) userNum);
            printf("The factorial of %d is %lld\n\n", userNum, result);
        }
    }
    
    putchar('\n');
    FindMaxFactorialByType(sizeof(short), 1);
    putchar('\n');
    FindMaxFactorialByType(sizeof(int), 1);
    putchar('\n');
    FindMaxFactorialByType(sizeof(long), 1);
    putchar('\n');
    FindMaxFactorialByType(sizeof(long long), 1);
    putchar('\n');
    FindMaxFactorialByType(sizeof(short), 0);
    putchar('\n');
    FindMaxFactorialByType(sizeof(int), 0);
    putchar('\n');
    FindMaxFactorialByType(sizeof(long), 0);
    putchar('\n');
    FindMaxFactorialByType(sizeof(long long), 0);
    putchar('\n');
    
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

void FindMaxFactorialByType(int sizeOfType, int isSigned) {
    /*
     Find the max factorial value for an int of a given size in bytes.
     isSigned specifies if that int is signed or unsigned.
     Print the max factorial value and the value n that produces that factorial.
     */
    
    int shifts;
    int n = 0;
    unsigned long long maxValue = 0x7FFFFFFFFFFFFFFF;
    unsigned long long maxFactorialValue = 0;
    char unsignedStr[strlen(UNSIGNED) + 1] = "";
    
    shifts = (sizeof(maxValue) - sizeOfType) * BITSINBYTE;
    maxValue = maxValue >> shifts;
    
    if (!isSigned) {
        maxValue *= 2;
        strncpy(unsignedStr, UNSIGNED, strlen(UNSIGNED));
    }
    
    int i, foundMax;
    for (i = 1, foundMax = 0; i <= MAXFACTORIAL + 1 && !foundMax; i++) {
        maxValue /= i;
        if (maxValue == 0) {
            foundMax = 1;
            n = i - 1;
            maxFactorialValue = Factorial(n);
        }
    }
    
    printf("For an %sinteger of size %d bytes,\n", unsignedStr, sizeOfType);
    printf("the max factorial value is %lld which is given by %d!\n", maxFactorialValue, n);
}


//HW #16, Noah Turnquist
//
//Enter 0 to exit.
//Max Value - 20
//Please enter a number: 3
//The factorial of 3 is 6
//
//
//Enter 0 to exit.
//Max Value - 20
//Please enter a number: 15
//The factorial of 15 is 1307674368000
//
//
//Enter 0 to exit.
//Max Value - 20
//Please enter a number: 22
//Please enter a number between 0 and 20.
//
//Enter 0 to exit.
//Max Value - 20
//Please enter a number: 250
//Please enter a valid number.
//
//Enter 0 to exit.
//Max Value - 20
//Please enter a number: -1
//Please enter a valid number.
//
//Enter 0 to exit.
//Max Value - 20
//Please enter a number: awdawd
//Please enter a valid number.
//
//Enter 0 to exit.
//Max Value - 20
//Please enter a number: 8
//The factorial of 8 is 40320
//
//
//Enter 0 to exit.
//Max Value - 20
//Please enter a number: 1
//The factorial of 1 is 1
//
//
//Enter 0 to exit.
//Max Value - 20
//Please enter a number: 20
//The factorial of 20 is 2432902008176640000
//
//
//Enter 0 to exit.
//Max Value - 20
//Please enter a number: 0
//
//For an integer of size 2 bytes,
//the max factorial value is 5040 which is given by 7!
//
//For an integer of size 4 bytes,
//the max factorial value is 479001600 which is given by 12!
//
//For an integer of size 8 bytes,
//the max factorial value is 2432902008176640000 which is given by 20!
//
//For an integer of size 8 bytes,
//the max factorial value is 2432902008176640000 which is given by 20!
//
//For an unsigned integer of size 2 bytes,
//the max factorial value is 40320 which is given by 8!
//
//For an unsigned integer of size 4 bytes,
//the max factorial value is 479001600 which is given by 12!
//
//For an unsigned integer of size 8 bytes,
//the max factorial value is 2432902008176640000 which is given by 20!
//
//For an unsigned integer of size 8 bytes,
//the max factorial value is 2432902008176640000 which is given by 20!
//
//Program ended with exit code: 0
