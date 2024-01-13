#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

void shellSort(int arr[], int n) {
    for (int d = n / 2; d > 0; d /= 2) {
        for (int i = d; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= d && arr[j - d] > temp; j -= d) {
                arr[j] = arr[j - d];
            }
            arr[j] = temp;
        }
    }
}

//MPI
void ParallelShellSort(int arr[], int n, int threads, int rank, int size) {
    int localSize = n / size;
    int* localArray = (int*)malloc(localSize * sizeof(int));
    //Делим на локальные массивы и отправляем
    MPI_Scatter(arr, localSize, MPI_INT, localArray, localSize, MPI_INT, 0, MPI_COMM_WORLD);
    //Сортируем полученную часть
    shellSort(localArray, localSize);
    //Собираем в порядке возрастания рангов - получаем отсортированный массив
    MPI_Gather(localArray, localSize, MPI_INT, arr, localSize, MPI_INT, 0, MPI_COMM_WORLD);
    free(localArray);
}

int lab6(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int count = 10000000;      /// Number of array elements
    const int seed = atoi(argv[1]);  ///seed
    int* arr = 0;
    srand(seed);
    if (!rank) {
        arr = (int*)malloc(count * sizeof(int));
        for (int i = 0; i < count; i++) {
            arr[i] = rand();
            //arr[i] = rand() % count;
        }
    }

    double start_time = MPI_Wtime();
    ParallelShellSort(arr, count, 1, rank, size);
    double end_time = MPI_Wtime();
    if (!rank) {
        printf("Estimated time: %.6f seconds\n", end_time - start_time);
    }
    free(arr);
    MPI_Finalize();
    return 0;
}
