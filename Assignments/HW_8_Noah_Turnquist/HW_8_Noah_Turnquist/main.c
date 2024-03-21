//HW #8, Noah Turnquist

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARY_SIZE 100
#define LOW_LIMIT 1
#define HIGH_LIMIT 999
#define NPERLINE 7
#define NEGMULT1 3
#define NEGMULT2 7

void PopulateArrayWithRandomNums(int arr[]);
void PrintAllArrayElements(int arr[], int arr_size);
int FindAndPrintAllOddValues(int arr[]);
void PrintArrayElementsAtOddIndices(int arr[]);
int GetSumOfAllArrayElements(int arr[]);
void FindSmallestAndGreatestElement(int arr[], int* small_index, int* large_index, int* small_value, int* large_value);
int GetNumOfPositives(int arr[]);
void PopulatePositiveAndNegativeArray(int base_arr[], int pos_arr[], int neg_arr[]);

int main(void) {
    int base_arr[ARY_SIZE];
    int odds;
    int sum;
    int small_index;
    int large_index;
    int small_value;
    int large_value;
    int positives;
    int negatives;
    
    printf("HW #8, Noah Turnquist\n\n");
    
    PopulateArrayWithRandomNums(base_arr);
    printf("The entire array:\n");
    PrintAllArrayElements(base_arr, ARY_SIZE);
    printf("\n");
    odds = FindAndPrintAllOddValues(base_arr);
    printf("\n");
    PrintArrayElementsAtOddIndices(base_arr);
    printf("\n");
    sum = GetSumOfAllArrayElements(base_arr);
    FindSmallestAndGreatestElement(base_arr, &small_index, &large_index, &small_value, &large_value);
    
    printf("Number of even numbers: %10d\n", ARY_SIZE - odds);
    printf("Sum of numbers:         %10d\n", sum);
    printf("Smallest value in the array: %5d at index: %d\n", small_value, small_index);
    printf("Largest value in the array:  %5d at index: %d\n", large_value, large_index);
    
    positives = GetNumOfPositives(base_arr);
    negatives = ARY_SIZE - positives;
    
    int positive_arr[positives];
    int negative_arr[negatives];
    PopulatePositiveAndNegativeArray(base_arr, positive_arr, negative_arr);
    
    printf("\n");
    printf("Positive values in the array are: \n");
    PrintAllArrayElements(positive_arr, positives);
    
    printf("\n");
    printf("Negative values in the array are: \n");
    PrintAllArrayElements(negative_arr, negatives);
    
    return 0;
}

void PopulateArrayWithRandomNums(int arr[]) {
    /*
     Create an array of ARY_SIZE random numbers in the range LOW_LIMIT ...
     HIGH_LIMIT, inclusively. In building the array, if the random number
     is evenly divided by NEGMULT1 or NEGMULT2, store it as a negative number.
     */
    
    int range = (HIGH_LIMIT - LOW_LIMIT) + 1;
    
    srand((unsigned int) time(0));
    
    for (int i = 0; i < ARY_SIZE; i++) {
        int num = rand() % range + LOW_LIMIT;
        if ((num % NEGMULT1 == 0) || (num % NEGMULT2 == 0)) {
            num *= -1;
        }
        arr[i] = num;
    }
}


void PrintAllArrayElements(int arr[], int arr_size) {
    /*
     Print all of the values in the array with each line of
     output containing NPERLINE values.
     */
    
    for (int i = 0; i < arr_size; i++) {
        printf("%4d ", arr[i]);
        if (i % NPERLINE == NPERLINE - 1) {
            printf("\n");
        }
    }
    printf("\n");
}


int FindAndPrintAllOddValues(int arr[]) {
    /*
     Print all of the odd values in the array with each line of
     output containing NPERLINE values. Also return the number
     of odd values.
     */
    
    int odds = 0;
    
    printf("Odd values in the array:\n");
    
    for (int i = 0; i < ARY_SIZE; i++) {
        if (abs(arr[i]) % 2 == 1) {
            printf("%4d ", arr[i]);
            odds++;
            if (odds % NPERLINE == NPERLINE - 1) {
                printf("\n");
            }
        }
    }
    printf("\n");
    
    return odds;
}


void PrintArrayElementsAtOddIndices(int arr[]) {
    /*
     Print all of the array elements that are at odd indices.
     Print NPERLINE values per line of output.
     */
    
    printf("Numbers located at odd indexes:\n");
    for (int i = 1; i < ARY_SIZE; i += 2) {
        printf("%4d ", arr[i]);
        if (i % NPERLINE == NPERLINE - 1) {
            printf("\n");
        }
    }
    printf("\n");
}


int GetSumOfAllArrayElements(int arr[]) {
    /*
     Sum all of the elements in an array. Return the sum.
     */
    
    int sum = 0;
    for (int i = 0; i < ARY_SIZE; i++) {
        sum += arr[i];
    }
    return sum;
}


void FindSmallestAndGreatestElement(int arr[], int* small_index, int* large_index, int* small_value, int* large_value) {
    /*
     Find the index and value of the smallest and greatest element in an array.
     Save those using pass by reference.
     */
    
    int min = arr[0];
    int max = arr[0];
    int min_index = 0;
    int max_index = 0;
    
    for (int i = 0; i < ARY_SIZE; i++) {
        if (arr[i] < min) {
            min = arr[i];
            min_index = i;
        }
        if (arr[i] > max) {
            max = arr[i];
            max_index = i;
        }
    }
    
    *small_index = min_index;
    *small_value = min;
    *large_index = max_index;
    *large_value = max;
}


int GetNumOfPositives(int arr[]) {
    /*
     Count the number of positive integers in an array
     */
    
    int positives = 0;
    
    for (int i = 0; i < ARY_SIZE; i++) {
        if (arr[i] >= 0) {
            positives++;
        }
    }
    
    return positives;
}


void PopulatePositiveAndNegativeArray(int base_arr[], int pos_arr[], int neg_arr[]) {
    /*
     Populate the positive and negative arrays using pass by reference
     with the positive and negative values from base_arr[].
    */
    
    int pos_index = 0;
    int neg_index = 0;
    
    for (int i = 0; i < ARY_SIZE; i++) {
        if (base_arr[i] >= 0) {
            pos_arr[pos_index] = base_arr[i];
            pos_index++;
        } else {
            neg_arr[neg_index] = base_arr[i];
            neg_index++;
        }
    }
}


//HW #8, Noah Turnquist
//
//The entire array:
// 313  533 -852  -33  800  443  530
// 710  919 -447  964 -549 -119  550
// 160  556  559  -12 -700  578   19
//-525 -678  676  956  176 -432  419
//-465  547 -609  575  743  319  -18
//-849 -663   61  -60  472  229  650
//  86    8  170  565  260  881  397
// 500  503  650  289  935   26 -846
// 997 -528  410  932  989 -350   10
// 277 -936  976  443 -354 -889  761
// 134 -429  220  -72  421 -990 -603
//-318   71  887    4  848  473 -990
// 250  668 -231  778  572   44  703
//-912 -854  391  625 -324 -705   97
// 787  752
//
//Odd values in the array:
// 313  533  -33  443  919 -447
//-549 -119  559   19 -525  419 -465
// 547 -609  575  743  319 -849 -663
//  61  229  565  881  397  503  289
// 935  997  989  277  443 -889  761
//-429  421 -603   71  887  473 -231
// 703  391  625 -705   97  787
//
//Numbers located at odd indexes:
// 533  -33  443  710 -447 -549  550
// 556  -12  578 -525  676  176  419
// 547  575  319 -849   61  472  650
//   8  565  881  500  650  935 -846
//-528  932 -350  277  976 -354  761
//-429  -72 -990 -318  887  848 -990
// 668  778   44 -912  391 -324   97
// 752
//
//Number of even numbers:         53
//Sum of numbers:              15905
//Smallest value in the array:  -990 at index: 75
//Largest value in the array:    997 at index: 56
//
//Positive values in the array are:
// 313  533  800  443  530  710  919
// 964  550  160  556  559  578   19
// 676  956  176  419  547  575  743
// 319   61  472  229  650   86    8
// 170  565  260  881  397  500  503
// 650  289  935   26  997  410  932
// 989   10  277  976  443  761  134
// 220  421   71  887    4  848  473
// 250  668  778  572   44  703  391
// 625   97  787  752
//
//Negative values in the array are:
//-852  -33 -447 -549 -119  -12 -700
//-525 -678 -432 -465 -609  -18 -849
//-663  -60 -846 -528 -350 -936 -354
//-889 -429  -72 -990 -603 -318 -990
//-231 -912 -854 -324 -705
//Program ended with exit code: 0
