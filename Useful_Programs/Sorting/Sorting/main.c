//
//  main.c
//  Sorting
//
//  Created by Noah Turnquist on 2/23/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 100000
#define MIN_RANDOM 1
#define MAX_RANDOM 100000

void GenerateRandomNumbers(int arr[], int min, int max, int arr_size);

void CopyArray(int first_arr[], int second_arr[], int arr_size);
void DisplayArray(int arr[], int arr_size);

void BubbleSort(int arr[], int arr_size, int output_time);

void InsertionSort(int arr[], int arr_size, int output_time);

void SelectionSort(int arr[], int arr_size, int output_time);

void MergeSort(int arr[], int arr_size, int output_time);
void Merge(int left_arr[], int right_arr[], int arr[], int arr_size); //Helper method for MergeSort()

void QuickSort(int arr[], int start, int end, int arr_size, int output_time);
int Partition(int arr[], int start, int end, int arr_size);

void RadixSort(int arr[], int arr_size, int output_time);
int GetMax(int arr[], int arr_size);
void CountSort(int arr[], int arr_size, int exp);

int BinarySearch(int arr[], int arr_size, int target, int output_time);


int main(void) {
    int random_numbers[ARRAY_SIZE];
//    int bubble_sorted_numbers[ARRAY_SIZE];
//    int insertion_sorted_numbers[ARRAY_SIZE];
    int selection_sorted_numbers[ARRAY_SIZE];
//    int merge_sorted_numbers[ARRAY_SIZE];
//    int quick_sorted_numbers[ARRAY_SIZE];
//    int radix_sorted_numbers[ARRAY_SIZE];
//    
//    int target_index;
    
    GenerateRandomNumbers(random_numbers, MIN_RANDOM, MAX_RANDOM, ARRAY_SIZE);
//    DisplayArray(random_numbers, ARRAY_SIZE);
    
//    CopyArray(random_numbers, bubble_sorted_numbers, ARRAY_SIZE);
//    BubbleSort(bubble_sorted_numbers, ARRAY_SIZE, 1);
//    
//    CopyArray(random_numbers, insertion_sorted_numbers, ARRAY_SIZE);
//    InsertionSort(insertion_sorted_numbers, ARRAY_SIZE, 1);
//    
    CopyArray(random_numbers, selection_sorted_numbers, ARRAY_SIZE);
    SelectionSort(selection_sorted_numbers, ARRAY_SIZE, 1);
//    
//    CopyArray(random_numbers, merge_sorted_numbers, ARRAY_SIZE);
//    MergeSort(merge_sorted_numbers, ARRAY_SIZE, 1);
//    
//    CopyArray(random_numbers, quick_sorted_numbers, ARRAY_SIZE);
//    QuickSort(quick_sorted_numbers, 0, ARRAY_SIZE - 1, ARRAY_SIZE, 1);
//    
//    CopyArray(random_numbers, radix_sorted_numbers, ARRAY_SIZE);
//    RadixSort(radix_sorted_numbers, ARRAY_SIZE, 1);
    
//    target_index = BinarySearch(merge_sorted_numbers, ARRAY_SIZE, 67982, 1);
//    printf("Target index: %d\n", target_index);
    
    return 0;
}


void GenerateRandomNumbers(int arr[], int min, int max, int arr_size) {
    int num;
    int range = (max - min) + 1;
    
    srand((unsigned int) time(0));
    
    for(int i = 0; i < arr_size; i++) {
        num = rand() % range + min;
        arr[i] = num;
    }
}

void CopyArray(int first_arr[], int second_arr[], int arr_size) {
    for(int i = 0; i < arr_size; i++) {
        second_arr[i] = first_arr[i];
    }
}

void DisplayArray(int arr[], int arr_size) {
    for(int i = 0; i < arr_size; i++) {
        printf("%d\n", arr[i]);
    }
}

void BubbleSort(int arr[], int arr_size, int output_time) {
    //Sort time is O(n^2) or quadratic time
    
    clock_t t;
    t = clock();
    
    for(int i = 0; i < arr_size; i++) {
        for(int j = 0; j < arr_size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    
    t = clock() - t;
    double time_taken = ((double) t / CLOCKS_PER_SEC);
    
    if (output_time) {
        printf("Bubble sort took %lf seconds to execute.\n", time_taken);
    }
}


void InsertionSort(int arr[], int arr_size, int output_time) {
    //Sort time is O(n^2) or quadratic time
    //Generally faster than Bubble Sort because it works front to back so it doesn't reorder
    //elements that already happen to be in order by change
    
    clock_t t;
    t = clock();
    
    for(int i = 1; i < arr_size; i++) {
        int current = arr[i];
        
        int j = i - 1;
        
        while (j > -1 && current < arr[j]) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = current;
    }
    
    t = clock() - t;
    double time_taken = ((double) t / CLOCKS_PER_SEC);
    
    if (output_time) {
        printf("Insertion sort took %lf seconds to execute.\n", time_taken);
    }
}


void SelectionSort(int arr[], int arr_size, int output_time) {
    //Sort time is O(n^2) or quadratic time
    
    clock_t t;
    t = clock();
    
    for(int i = 0; i < arr_size; i++) {
        int min = i;
        
        for(int j = i + 1; j < arr_size; j++) {
            if(arr[j] < arr[min]) {
                min = j;
            }
        }
        if(min != i) {
            int temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;
        }
    }
    
    t = clock() - t;
    double time_taken = ((double) t / CLOCKS_PER_SEC);
    
    if (output_time) {
        printf("Selection sort took %lf seconds to execute.\n", time_taken);
    }
}


void MergeSort(int arr[], int arr_size, int output_time) {
    //Sort time is O(n log(n)) or quasilinear time
    //Much more memory intensive than some other sorting algorithms
    
    clock_t t;
    t = clock();
    
    int length = arr_size;
    if(length <= 1) return; //base case
    
    int middle = length / 2;
    int left_arr[middle];
    int right_arr[length - middle];
    
    int i = 0; //left array
    int j = 0; //right array
    
    for(; i < length; i++) {
        if(i < middle) {
            left_arr[i] = arr[i];
        } else {
            right_arr[j] = arr[i];
            j++;
        }
    }
    
    MergeSort(left_arr, (int) sizeof(left_arr) / sizeof(int), 0);
    MergeSort(right_arr, (int) sizeof(right_arr) / sizeof(int), 0);
    Merge(left_arr, right_arr, arr, arr_size);
    
    t = clock() - t;
    double time_taken = ((double) t / CLOCKS_PER_SEC);
    
    if (output_time) {
        printf("Merge sort took %lf seconds to execute.\n", time_taken);
    }
}


//Helper method for MergeSort()
void Merge(int left_arr[], int right_arr[], int arr[], int arr_size) {
    int left_size = arr_size / 2;
    int right_size = arr_size - left_size;
    
    //indices
    int i = 0;
    int l = 0;
    int r = 0;
    
    //Check conditions for mergin
    while(l < left_size && r < right_size) {
        if(left_arr[l] < right_arr[r]) {
            arr[i] = left_arr[l];
            i++;
            l++;
        } else {
            arr[i] = right_arr[r];
            i++;
            r++;
        }
    }
    while(l < left_size) {
        arr[i] = left_arr[l];
        i++;
        l++;
    }
    while(r < right_size) {
        arr[i] = right_arr[r];
        i++;
        r++;
    }
}


void QuickSort(int arr[], int start, int end, int arr_size, int output_time) {
    //Alternative to merge sort that uses less memory
    //Moves smaller elements to the left side of a pivot and recursively divides array
    //      into 2 partitions
    //Run time complexity of best case and average time of O(n log(n)) or quasilinear time
    //      worst case can run at O(n^2) or quadratic time if the array happens to already be sorted
    
    clock_t t;
    t = clock();
    
    if (end <= start) return;
    
    int pivot = Partition(arr, start, end, arr_size);
    QuickSort(arr, start, pivot - 1, pivot - start, 0);
    QuickSort(arr, pivot + 1, end, end - pivot, 0);
    
    t = clock() - t;
    double time_taken = ((double) t / CLOCKS_PER_SEC);
    
    if (output_time) {
        printf("Quick Sort took %lf seconds to execute.\n", time_taken);
    }
}

int Partition(int arr[], int start, int end, int arr_size) {
    int pivot = arr[end];
    int i = start - 1;
    
    for(int j = start; j <= end - 1; j++) {
        if(arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    i++;
    int temp = arr[i];
    arr[i] = arr[end];
    arr[end] = temp;
    
    
    return i;
}


void RadixSort(int arr[], int arr_size, int output_time) {
    //A pretty complex sorting algorithm that only works with numbers
    //The idea is that it sorts the numbers by sorting them by their individual digits
    //      so starting by looking at the ones place, and then the tens place, etc.
    //Time complexity: O(n * d), where d is the number of digits in the largest number
    
    clock_t t;
    t = clock();
    
    int m = GetMax(arr, arr_size);
    for (int exp = 1; m / exp > 0; exp *= 10) {
        CountSort(arr, arr_size, exp);
    }
    
    t = clock() - t;
    double time_taken = ((double) t / CLOCKS_PER_SEC);
    
    if (output_time) {
        printf("Radix sort took %lf seconds to execute.\n", time_taken);
    }
}


int GetMax(int arr[], int arr_size) {
    int max = arr[0];
    for(int i = 1; i < arr_size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}


void CountSort(int arr[], int arr_size, int exp) {
    int output[arr_size];
    int i, count[10] = { 0 };
    
    for (i = 0; i < arr_size; i++) {
        count[(arr[i] / exp) % 10]++;
    }
    
    for (i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    
    for (i = arr_size - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
    
    for (i = 0; i < arr_size; i++) {
        arr[i] = output[i];
    }
}


int BinarySearch(int arr[], int arr_size, int target, int output_time) {
    //Binary search method. Returns index of target element. Returns -1 if element not found.
    
    clock_t t;
    t = clock();
    
    int first = 0;
    int middle = arr_size / 2;
    int last = arr_size;
    
    while (first <= last) {
        middle = (first + last) / 2;
        
        if (target > arr[middle]) {
            first = middle + 1;
        } else if (target < arr[middle]) {
            last = middle - 1;
        } else {
            first = last + 1;
        }
    }
    
    t = clock() - t;
    double time_taken = ((double) t / CLOCKS_PER_SEC);
    
    if (output_time) {
        printf("Binary Search took %lf seconds to execute.\n", time_taken);
    }
    
    if (target == arr[middle]) {
        return middle;
    } else {
        return -1;
    }
}
