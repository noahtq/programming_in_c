//HW #6, Noah Turnquist

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#define NAVGS 6

int GenerateRandomInt(void); //Generate Random Int in the range 5 to 50
double GenerateRandomDouble(void);

int main(void) {
    int numPoints;
    double sum;
    double average;
    double max_average = -DBL_MAX; //Initialize to the smallest possible double value
    
    srand((unsigned int) time(0));
    
    printf("HW #6, Noah Turnquist\n");
    printf("Set #   N Points   Average\n");
    printf("=====   ========   =======\n");
    
    for (int i = 0; i < NAVGS; i++) {
        numPoints = GenerateRandomInt();
        
        sum = 0;
        for(int j = 0; j < numPoints; j++) {
            sum += GenerateRandomDouble();
        }
        
        average = sum / numPoints;
        
        if (average > max_average) {
            max_average = average;
        }
        
        printf("  %d        %2d      %2.3lf\n", i + 1, numPoints, average);
    }
    
    printf("The maximum average was %.3lf\n", max_average);
    
    return 0;
}

int GenerateRandomInt(void) {
    int range = (50 - 5) + 1;
    return rand() % range + 5;
}

double GenerateRandomDouble(void) {
    return rand() * -10.0 / RAND_MAX;
}

//OUTPUT #1

//HW #6, Noah Turnquist
//Set #   N Points   Average
//=====   ========   =======
//  1        16      -3.902
//  2        11      -5.545
//  3        36      -4.357
//  4        11      -4.115
//  5         7      -4.237
//  6         5      -4.633
//The maximum average was -3.902
//Program ended with exit code: 0


//OUTPUT #2

//HW #6, Noah Turnquist
//Set #   N Points   Average
//=====   ========   =======
//  1        37      -5.324
//  2         7      -4.851
//  3        10      -4.627
//  4        31      -5.106
//  5        18      -5.781
//  6        50      -5.069
//The maximum average was -4.627
//Program ended with exit code: 0


//OUTPUT #3 - Changed program for this output to verify average and sum is functioning correctly
//          - Placed print statements in different spots and commented out other print statements
//          - Switched the value of NAVGS to 2 to reduce the length of the output

//HW #6, Noah Turnquist
//N Points: 21
//Sum: -1.864728
//Sum: -2.346846
//Sum: -5.299916
//Sum: -7.560684
//Sum: -14.286037
//Sum: -17.285159
//Sum: -23.539802
//Sum: -25.323136
//Sum: -27.804195
//Sum: -36.977994
//Sum: -41.003908
//Sum: -44.540355
//Sum: -51.613685
//Sum: -53.076358
//Sum: -56.210681
//Sum: -64.778462
//Sum: -73.470844
//Sum: -76.341469
//Sum: -82.940508
//Sum: -82.975375
//Sum: -88.993791
//Average: -4.237800
//
//N Points: 35
//Sum: -4.620286
//Sum: -7.767875
//Sum: -9.293498
//Sum: -10.440637
//Sum: -20.398114
//Sum: -25.714891
//Sum: -34.792173
//Sum: -36.679480
//Sum: -46.641582
//Sum: -49.681954
//Sum: -59.217908
//Sum: -60.003880
//Sum: -69.839612
//Sum: -78.987053
//Sum: -80.012839
//Sum: -80.411992
//Sum: -88.962296
//Sum: -93.923135
//Sum: -100.751581
//Sum: -106.447264
//Sum: -113.794412
//Sum: -117.297840
//Sum: -119.410213
//Sum: -122.068907
//Sum: -126.739064
//Sum: -128.064269
//Sum: -130.786923
//Sum: -140.439108
//Sum: -144.701639
//Sum: -145.064325
//Sum: -150.725704
//Sum: -151.528333
//Sum: -161.315641
//Sum: -166.606299
//Sum: -166.688599
//Average: -4.762531
//
//Program ended with exit code: 0


//VERIFICATION PLAN

//TEST CASE 1 - GenerateRandomInt() should return values between 5 and 50 including 5 and 50
//Input - None
//Expected Output - All 6 numbers in N points are between 5 and 50 with 5 and 50 being included
//Result - Output #1 produces numbers between 5 and 50 with 5 being one of the numbers
//       - Output #2 produces numbers between 5 and 50 with 50 being one of the numbers

//TEST CASE 2 - Verify that the average is being calculated correctly
//            - and that the nested sum loop is running the correct number of times
//            - Note: changed print statements in the program for this test case to ensure average
//            -       and sum are working as expected
//Input - None
//Expected Output - If N Points has the value of n, the nested loop should have n iterations
//                - which can be verified by the number of times the sum value is updated.
//                - Average should equal the sum divided by N Points
//Result - Output #3 correctly sums a total of 21 doubles for the first Set and then 35 doubles for the second set
//       - The first set of Output #3 correctly produces an average of -4.23 for the sum -88.993 / 21
//       - The second set of Output #3 correctly produces an average of -4.76 for the sum -166.688 / 35

//TEST CASE 3 - Verify the correct maximum average is printed
//Input - None
//Expected Output - The largest number from the average column is output underneath the table
//Result - Output #1 correctly outputs -3.902 as the maximum average
//       - Output #2 correctly outputs -4.672 as the maximum average

