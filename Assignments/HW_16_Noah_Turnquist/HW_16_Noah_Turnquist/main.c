//HW #16, Noah Turnquist

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXFACTORIAL 20
#define EXITVAL 0
#define FLUSH while(getchar() != '\n')

int GetNumFromUser(void);
unsigned long long Factorial(unsigned long long n);

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
    
    //Print out max factorial value that can be held in a 4-byte int
    //Print out max facrotial value that can be held in a 8-byte int
    
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

//n * (n - 1) * (n - 2) * ... * 1
unsigned long long Factorial(unsigned long long n) {
    if (n <= 1) {
        return 1;
    }
    return n * Factorial(n - 1);
}
