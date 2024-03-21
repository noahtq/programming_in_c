// HW #9, Noah Turnquist

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARY_SIZE 100
#define LOW_LIMIT 1
#define HIGH_LIMIT 999
#define NPERLINE 10
#define NEGMULT1 3
#define NEGMULT2 7

int* CreatePointerArrayWithRandomNums(void);
void PrintAllPointerArrayElements(int *pArr, int ary_size);
int FindAndPrintAllOddPointerArrayValues(int *pArr);
void PrintPointerArrayElementsAtOddIndices(int *pArr);
int GetSumOfAllPointerArrayElements(int *pArr);
int FindPointerArrayIndexSmallOrLarge(int *pArr, int find_largest);
int GetNumOfPositivesInPointerArray(int *pArr);
void PopulatePointerArrayOnlyPositiveOrNegative(int *pArr, int elements, int add_negatives);

int main(void) {
    printf("HW #9, Noah Turnquist\n\n");
    
    int odds;
    int sum;
    int small_index;
    int large_index;
    int positives;
    int negatives;
    
    int *pArr = CreatePointerArrayWithRandomNums();
    if (pArr == NULL) {
        printf("Couldn't allocate memory on the heap for the requested pointer.\n");
        return 1;
    }
    
    printf("The entire array:\n");
    PrintAllPointerArrayElements(pArr, ARY_SIZE);
    putchar('\n');
    odds = FindAndPrintAllOddPointerArrayValues(pArr);
    putchar('\n');
    PrintPointerArrayElementsAtOddIndices(pArr);
    putchar('\n');
    sum = GetSumOfAllPointerArrayElements(pArr);
    small_index = FindPointerArrayIndexSmallOrLarge(pArr, 0);
    large_index = FindPointerArrayIndexSmallOrLarge(pArr, 1);
    
    printf("Number of even numbers: %10d\n", ARY_SIZE - odds);
    printf("Sum of numbers:         %10d\n", sum);
    printf("Smallest value in the array: %5d at index: %d\n", *(pArr + small_index), small_index);
    printf("Largest value in the array:  %5d at index: %d\n", *(pArr + large_index), large_index);
    
    positives = GetNumOfPositivesInPointerArray(pArr);
    negatives = ARY_SIZE - positives;
    
    putchar('\n');
    printf("Positive values in the array are: \n");
    PopulatePointerArrayOnlyPositiveOrNegative(pArr, positives, 0);
    putchar('\n');
    
    printf("Negative values in the array are: \n");
    PopulatePointerArrayOnlyPositiveOrNegative(pArr, negatives, 1);
    putchar('\n');
    
    free(pArr);
    pArr = NULL;
    
    return 0;
}

int* CreatePointerArrayWithRandomNums(void) {
    /*
     Create an array on the heap, return a pointer to the array from the function.
     Populate with random numbers in the range LOW_LIMIT ...
     HIGH_LIMIT, inclusively. In populating the array, if the random number
     is evenly divided by NEGMULT1 or NEGMULT2, store it as a negative number.
     */
    
    int* pArr = (int*) malloc(ARY_SIZE * sizeof(int));
    if (pArr == NULL) {
        return NULL;
    }
    int range = (HIGH_LIMIT - LOW_LIMIT) + 1;
    
    srand((unsigned int) time(0));
    
    int* end = pArr + ARY_SIZE;
    int* itr = pArr;
    while (itr != end) {
        int num = rand() % range + LOW_LIMIT;
        if ((num % NEGMULT1 == 0) || (num % NEGMULT2 == 0)) {
            num *= -1;
        }
        *itr = num;
        itr++;
    }
    return pArr;
}

void PrintAllPointerArrayElements(int *pArr, int ary_size) {
    /*
     Print all of the values in the pointer array with each line of
     output containing NPERLINE values.
     */
    
    int* end = pArr + ary_size;
    int* itr = pArr;
    int n = 0;
    while(itr != end) {
        printf("%4d ", *itr);
        itr++;
        if (++n == NPERLINE) {
            putchar('\n');
            n = 0;
        }
    }
    printf("\n");
}

int FindAndPrintAllOddPointerArrayValues(int *pArr) {
    /*
     Print all of the odd values in the array referenced by the pointer
     with each line of output containing NPERLINE values.
     Also return the number of odd values.
     */
    
    printf("Odd values in the array:\n");
    
    int odds = 0;
    int* end = pArr + ARY_SIZE;
    int* itr = pArr;
    int n = 0;
    while (itr != end) {
        if (abs(*itr) % 2 == 1) {
            printf("%4d ", *itr);
            odds++;
            if (++n == NPERLINE) {
                putchar('\n');
                n = 0;
            }
        }
        itr++;
    }
    printf("\n");
    
    return odds;
}

void PrintPointerArrayElementsAtOddIndices(int *pArr) {
    /*
     Print all of the array elements that are at odd indices.
     Print NPERLINE values per line of output.
     */
    
    printf("Numbers located at odd indexes:\n");
    
    int* end = pArr + ARY_SIZE;
    int* itr = pArr + 1;
    int n = 0;
    while ((itr != end) && (itr != end + 1)) {
        printf("%4d ", *itr);
        itr += 2;
        if (++n == NPERLINE) {
            putchar('\n');
            n = 0;
        }
    }
    printf("\n");
}

int GetSumOfAllPointerArrayElements(int *pArr) {
    /*
     Sum all of the elements in an pointer array. Return the sum.
     */
    
    int sum = 0;
    int* end = pArr + ARY_SIZE;
    int* itr = pArr;
    while (itr != end) {
        sum += *(itr++);
    }
    return sum;
}

int FindSmallestPointerArrayElement(int *pArr) {
    /*
     Find the smallest value in an array referenced by the pointer pArr.
     */
    
    int* end = pArr + ARY_SIZE;
    int* itr = pArr;
    
    int min = *itr;
    
    while (itr != end) {
        if (*itr < min) {
            min = *itr;
        }
        itr++;
    }
    return min;
}

int FindPointerArrayIndexSmallOrLarge(int *pArr, int find_largest) {
    /*
     Depending on if find_largest is true or false, find and return the index
     of the smallest or largest element in the array referenced by the
     pointer pArr.
     */
    
    int* end = pArr + ARY_SIZE;
    int* itr = pArr;
    
    int target_value = *itr;
    int* location = itr;
    
    while (itr != end) {
        if (find_largest) {
            if (*itr > target_value) {
                target_value = *itr;
                location = itr;
            }
        } else {
            if (*itr < target_value) {
                target_value = *itr;
                location = itr;
            }
        }
        itr++;
    }
    return (int) (location - pArr);
}

int GetNumOfPositivesInPointerArray(int *pArr) {
    /*
     Find the total number of positive elements in a pointer array
     */
    
    int positives = 0;
    int* end = pArr + ARY_SIZE;
    int* itr = pArr;
    
    while (itr != end) {
        if (*itr >= 0) {
            positives++;
        }
        itr++;
    }
    return positives;
}

void PopulatePointerArrayOnlyPositiveOrNegative(int *pArr, int elements, int add_negatives) {
    /*
     Create a new array on the heap referenced by a pointer.
     Fill that array with either positive or negative values depending
     on the truthiness of add_negatives.
     Print out the new array using the PrintPointerArray() function.
     */
    
    int* new_arr = (int*) malloc(elements * sizeof(int));
    if (new_arr == NULL) {
        printf("Couldn't allocate memory.\n");
    } else {
        int* end = pArr + ARY_SIZE;
        int* itr = pArr;
        int* new_arr_itr = new_arr;
        
        while (itr != end) {
            if (add_negatives) {
                if (*itr < 0) {
                    *new_arr_itr = *itr;
                    new_arr_itr++;
                }
            } else {
                if (*itr >= 0) {
                    *new_arr_itr = *itr;
                    new_arr_itr++;
                }
            }
            itr++;
        }
        PrintAllPointerArrayElements(new_arr, elements);
        
        free(new_arr);
        new_arr = NULL;
    }
}


//HW #9, Noah Turnquist
//
//The entire array:
// 121  -14  971  968 -993  397  904 -696  151  800
//  29 -435  349  733  635 -222  130  247 -171  103
// -39 -161  122  604 -279 -280 -630  412  995  254
// 638  229 -665 -910  955 -774 -279  898  470  676
//  59 -435 -434  692  397 -204  394   89  332 -198
// 907  848 -573 -423  286  811   -9  499 -669  659
// -63  647 -294  499 -645  785  436 -119  422  710
// 754 -990  871  178   29 -777 -666  250 -939 -381
// 619 -132  898 -480  -18  862  998  370 -507 -711
//-153 -672  968  170  925 -700  284 -966  842 -175
//
//
//Odd values in the array:
// 121  971 -993  397  151   29 -435  349  733  635
// 247 -171  103  -39 -161 -279  995  229 -665  955
//-279   59 -435  397   89  907 -573 -423  811   -9
// 499 -669  659  -63  647  499 -645  785 -119  871
//  29 -777 -939 -381  619 -507 -711 -153  925 -175
//
//
//Numbers located at odd indexes:
// -14  968  397 -696  800 -435  733 -222  247  103
//-161  604 -280  412  254  229 -910 -774  898  676
//-435  692 -204   89 -198  848 -423  811  499  659
// 647  499  785 -119  710 -990  178 -777  250 -381
//-132 -480  862  370 -711 -672  170 -700 -966 -175
//
//
//Number of even numbers:         50
//Sum of numbers:              12400
//Smallest value in the array:  -993 at index: 4
//Largest value in the array:    998 at index: 86
//
//Positive values in the array are:
// 121  971  968  397  904  151  800   29  349  733
// 635  130  247  103  122  604  412  995  254  638
// 229  955  898  470  676   59  692  397  394   89
// 332  907  848  286  811  499  659  647  499  785
// 436  422  710  754  871  178   29  250  619  898
// 862  998  370  968  170  925  284  842
//
//Negative values in the array are:
// -14 -993 -696 -435 -222 -171  -39 -161 -279 -280
//-630 -665 -910 -774 -279 -435 -434 -204 -198 -573
//-423   -9 -669  -63 -294 -645 -119 -990 -777 -666
//-939 -381 -132 -480  -18 -507 -711 -153 -672 -700
//-966 -175
//
//Program ended with exit code: 0
