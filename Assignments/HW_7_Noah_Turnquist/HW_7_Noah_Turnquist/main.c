#include <stdio.h>
#include <string.h>

#define ASSIGNMENT "Homework #7, Noah Turnquist"
#define FLUSH while(getchar() != '\n')

char GetOperation(void);
double GetOperator(char prompt[50]);
void CalculateAndPrintResult(char, double, double);

int main(void) {
    printf("%s\n", ASSIGNMENT);
    
    char operation;
    double num1;
    double num2;
    
    do {
        operation = GetOperation();
        if (operation != 'q') {
            num1 = GetOperator("Enter the first number");
            num2 = GetOperator("Enter the second number");
            if (operation == 'd' && num2 == 0) {
                printf("Sorry. You can't divide by 0.\n");
            } else {
                CalculateAndPrintResult(operation, num1, num2);
            }
        }
    } while (operation != 'q');
    
    printf("Bye\n");
    
    return 0;
}

char GetOperation(void) {
    int valid_input = 0;
    char operation;
    
    printf("Enter the operation of your choice:\n");
    printf("a. add          s. subtract\n");
    printf("m. multiply     d. divide\n");
    printf("q. quit\n");
    
    do {
        scanf(" %c", &operation);
        FLUSH;
        //Test to make sure operation is valid
        if (operation == 'a' || operation == 's' || operation == 'm' ||
            operation == 'd' || operation == 'q') {
            valid_input = 1;
        } else {
            printf("That is not a valid operation. Please try again.\n");
        }
    } while(valid_input != 1);
    
    return operation;
}

double GetOperator(char prompt[50]) {
    int valid_input = 0;
    double operator;
    
    printf("%s: ", prompt);
    do {
        //Test to make sure operation is valid
        if(scanf(" %lf", &operator) != 1) {
            FLUSH;
            printf("That is not a valid floating point number. Please try again: ");
        } else {
            FLUSH;
            valid_input = 1;
        }
    } while(valid_input != 1);
    
    return operator;
}

void CalculateAndPrintResult(char operation, double num1, double num2) {
    double result = 0;
    
    switch(operation) {
        case 'a':
            result = num1 + num2;
            printf("%g + %g = %g\n", num1, num2, result);
            break;
        case 's':
            result = num1 - num2;
            printf("%g - %g = %g\n", num1, num2, result);
            break;
        case 'm':
            result = num1 * num2;
            printf("%g * %g = %g\n", num1, num2, result);
            break;
        case 'd':
            result = num1 / num2;
            printf("%g / %g = %g\n", num1, num2, result);
            break;
    }
    printf("\n");
}


//SAMPLE OUTPUT

//Homework #7, Noah Turnquist
//Enter the operation of your choice:
//a. add          s. subtract
//m. multiply     d. divide
//q. quit
//e
//
//That is not a valid operation. Please try again.
//ertd
//
//That is not a valid operation. Please try again.
//4.5
//
//That is not a valid operation. Please try again.
//a
//
//Enter the first number: 4.5
//Enter the second number: 2
//4.5 + 2 = 6.5
//
//Enter the operation of your choice:
//a. add          s. subtract
//m. multiply     d. divide
//q. quit
//s
//
//Enter the first number: adafavacaw
//That is not a valid floating point number. Please try again: vvvv
//That is not a valid floating point number. Please try again: 4.5
//Enter the second number: 2
//4.5 - 2 = 2.5
//
//Enter the operation of your choice:
//a. add          s. subtract
//m. multiply     d. divide
//q. quit
//m
//
//Enter the first number: 4.5
//Enter the second number: -2
//4.5 * -2 = -9
//
//Enter the operation of your choice:
//a. add          s. subtract
//m. multiply     d. divide
//q. quit
//d
//
//Enter the first number: 15
//Enter the second number: 0
//Sorry. You can't divide by 0.
//Enter the operation of your choice:
//a. add          s. subtract
//m. multiply     d. divide
//q. quit
//d
//
//Enter the first number: 15
//Enter the second number: 3.0
//15 / 3 = 5
//
//Enter the operation of your choice:
//a. add          s. subtract
//m. multiply     d. divide
//q. quit
//q
//
//Bye
//Program ended with exit code: 0


//VERIFICATION PLAN

//TEST CASE 1 - Safely handle user entering bad input in GetOperation().
//Also handle user entering extra input after operation.
//Input - 'v' 'bgbdbseadwa' 'a 3.2'
//Expected Result - 'Enter the operation of your choice:...'
//'That is not a valid operation. Please try again.'
//'That is not a valid operation. Please try again.'
//'Enter the first number: '

//TEST CASE 2 - Safely handle user entering bad input in GetOperator()
//Input - 'a' 'adwwgafwad' 'r' '5.7'
//Expected Result - 'Enter the operation of your choice:...' 'Enter the first number:'
// 'That is not a valid floating point number. Please try again:'
// 'That is not a valid floating point number. Please try again:'
// 'That is not a valid floating point number. Please try again:'
// 'Enter the second number:'

//TEST CASE 3 - Make sure user can't divide by 0
//Input - 'd' '5.5' '0'
//Expected Result - 'Enter the operation of your choice:...'
//'Enter the first number:'
//'Enter the second number:'
//'Sorry. You can't divide by 0.'
//'Enter the operation of your choice:...'

//TEST CASE 4 - Test to make sure each operation in CalculateAndPrintResult() returns the expected result
//Input - 'a' '5.5' '2'
// 's' '5.5' '2.0'
// 'm' '5.5' '-2'
// 'd' '5.5' '2.0'
// 'q'
//Expected Result - 'Enter the operation of your choice:...'
//'Enter the first number:'
//'Enter the second number:'
//'5.5 + 2 = 7.5'
// ...
// 5.5 - 2 = 3.5
// ...
// 5.5 * -2 = -11
// ...
// 5.5 / 2 = 2.75
// ...
// Bye
