//Homework #2, Noah Turnquist

#include <stdio.h>

int main(void) {
    //Local Declarations
    double cups, pints, ounces, tablespoons, teaspoons;
    int z, x=3;
    z = x * 2 + x++;
    printf("%d", z);
    printf("Homework #2, Noah Turnquist\n");
    //Get number of cups from the user
    printf("Enter the number of cups to be converted: ");
    int input_result = scanf("%lf", &cups);
    
    //Handle incorrect user inputs
    if (input_result == 1) {
        //Conversions to various measurements
        pints = cups / 2;
        ounces = cups * 8;
        tablespoons = ounces * 2;
        teaspoons = tablespoons * 3;
        
        //Formatted Output
        printf("%10.2lf cups = %10.3lf pints\n", cups, pints);
        printf("                = %10.3lf ounces\n", ounces);
        printf("                = %10.3lf tablespoons\n", tablespoons);
        printf("                = %10.3lf teaspoons\n", teaspoons);
    } else {
        printf("Error invalid input entered.\n");
    }
    return 0;
}

//Homework #2, Noah Turnquist
//Enter the number of cups to be converted: 520.56
//    520.56 cups =    260.280 pints
//                =   4164.480 ounces
//                =   8328.960 tablespoons
//                =  24986.880 teaspoons
//Program ended with exit code: 0
