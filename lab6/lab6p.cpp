#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#pragma warning(disable : 4996)

//OMP
void shellSortParallelOMP(int arr[], int n, int threads) {
    int i, j, temp, part;
    for (int gap = n / 2; gap > 0; gap /= 2) {
#pragma omp parallel for shared(gap, n, arr) private(i, j, temp, part) default(none) num_threads(threads)
        for (i = 0; i < gap; i++) {
            for (part = i + gap; part < n; part += gap) {
                for (j = part; j > i && arr[j - gap] > arr[j]; j -= gap) {
                    temp = arr[j];
                    arr[j] = arr[j - gap];
                    arr[j - gap] = temp;
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    const int count = 10000000;     /// Number of array elements
    int seeds[] = {123, 9997777, 88801, 5106803, 98765, 67676766, 4879, 5103, 99999, 100000}; ///seeds
    int* arr = (int*)malloc(sizeof(int) * count);
    double start, end;
    for (int k = 2; k < 18; k += 2) {
        printf("Threads = %d\n", k);
        for (int j = 0; j < 10; j++) {
            srand(seeds[j]);
            for (int i = 0; i < count; i++) {
                arr[i] = rand();
            }
            start = omp_get_wtime();
            shellSortParallelOMP(arr, count, k);
            end = omp_get_wtime();
            printf("Estimated time : % .6f seconds\n", end - start);
        }
    }
}
