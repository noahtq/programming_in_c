//HW #5, Noah Turnquist

#include <stdio.h>
#include <ctype.h>


void SumNumbers(int n);
void Conditional(char ch);

int main(void) {
    int n;
    char ch;
    
    printf("HW #5, Noah Turnquist\n");
    
    printf("Enter a positive integer less than 20: ");
    if (scanf("%d", &n) == 1) {
        SumNumbers(n);
    } else {
        printf("Error: something went wrong.\n");
        return 1;
    }
    
    printf("Enter any character (a '#' will exit the program): ");
    scanf(" %c", &ch);
    while(ch != '#') {
        Conditional(ch);
        printf("Enter any character (a '#' will exit the program): ");
        scanf(" %c", &ch);
    }
    
    return 0;
}

void SumNumbers(int n) {
    int count = 0;
    for(int i = 1; i <= n; i++) {
        count += i;
    }
    printf("You entered: %d. The sum of that number starting at 0 is: %d\n", n, count);
}

void Conditional(char ch) {
    printf("Running if function with %c.\n", ch);
    
    if (islower(ch) && ch != 'a') {
        ch = toupper(ch);
    } else if (isupper(ch) && ch != 'A') {
        ch = tolower(ch);
    } else if (isdigit(ch)) {
        ch += 64;
    }
    printf("If function: New value of character: %c\n", ch);
}


//HW #5, Noah Turnquist
//Enter a positive integer less than 20: 15
//You entered: 15. The sum of that number starting at 0 is: 120
//Enter any character (a '#' will exit the program): r
//Running if function with r.
//If function: New value of character: R
//Enter any character (a '#' will exit the program): T
//Running if function with T.
//If function: New value of character: t
//Enter any character (a '#' will exit the program): 3
//Running if function with 3.
//If function: New value of character: s
//Enter any character (a '#' will exit the program): #
//Program ended with exit code: 0


//VERIFICATION PLAN

//TEST CASE 1 - Verify that first Scanf exits program and prints error message if an integer is not entered.
//INPUT: 'a'
//EXPECTED OUTPUT: "Error: something went wrong"
//                 "Program ended with exit code: 1"

//TEST CASE 2 - Verify that the number sum for loop produces expected output, and verify program can be exited on first while loop iterartion by entering '#'
//INPUT: '5', '#'
//EXPECTED OUTPUT: 15

//TEST CASE 3 - Verify that the while loop can run for multiple iterations, accepting multiple different inputs
//INPUT: '10', 's', '5', 'T', '#'
//EXPECTED OUTPUT: 55, 'S', 'u', 'T'

//TEST CASE 4 - Verify that the while loop can run for multiple iterations, accepting the same input  inputs
//INPUT: '3', 'a', 'a', 'a', '#'
//EXPECTED OUTPUT: 6, 'a', 'a', 'a'

