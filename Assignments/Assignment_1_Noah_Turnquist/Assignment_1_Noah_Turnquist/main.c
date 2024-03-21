//Homework #1, Noah Turnquist

#include <stdio.h>

int main(void) {
    //Local declarations
    char character;
    int number;
    double decimal_number;
    
    //Print some info and a prompt for user input
    printf("HW #01, Noah Turnquist\n\n");
    printf("Enter character, an integer, and a floating-point number seperated by a space: ");
    //Read in specified user input
    scanf("%c %d %lf", &character, &number, &decimal_number);
    //Output the information the user entered
    printf("%c\n", character);
    printf("%d\n", character);
    printf("%d\n", number);
    printf("%lf\n", decimal_number);
    
    return 0;
}

//HW #01, Noah Turnquist
//
//Enter character, an integer, and a floating-point number seperated by a space: b 12524 -1.0567
//b
//98
//12524
//-1.056700
//Program ended with exit code: 0
