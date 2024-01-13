#include <iostream>
#include <omp.h>
#include <mpi.h>

int PrimeCheck(int N) {
    for (int i = 2; i * i < N + 1; i++) {
        if (!(N % i)) return 0;
    }
    return 1;
}

void ThreadWork(int* chunk, int size, int thread_count) {
    #pragma omp parallel for num_threads(thread_count) schedule(guided) 
    for (int i = 0; i < size; i++) {
        if (PrimeCheck(chunk[i])) chunk[i] = 1; // 1 - prime
        else chunk[i] = 0;                      // 0 - not a prime
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size, lcount = 0,
        lbound = atoi(argv[1]),
        ubound = atoi(argv[2]),
        count = ubound - lbound + 1;
    int* arr = 0;
    double time;
    double* times = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (!rank) {
        arr = (int*)malloc(sizeof(int) * count);
        for (int i = lbound; i < ubound + 1; i++) {
            arr[i - lbound] = i;
        }
        times = (double*)malloc(sizeof(double)*size);
    }
    
    //Work
    lcount = count / size;
    int* local_arr = (int*)malloc(lcount * sizeof(int));
    //Split array
    MPI_Scatter(arr, lcount, MPI_INT, local_arr, lcount, MPI_INT, 0, MPI_COMM_WORLD);
    //Thread result
    time = MPI_Wtime();
    ThreadWork(local_arr, lcount, 2); // 16 threads, 8 procs
    time = MPI_Wtime() - time;
    //Get all results
    MPI_Gather(local_arr, lcount, MPI_INT, arr, lcount, MPI_INT, 0, MPI_COMM_WORLD);
    //Get times
    MPI_Gather(&time, 1, MPI_DOUBLE, times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    free(local_arr);
    if (!rank) {
        printf("Primes: \n");
        for (int i = 0; i < count; i++) {
            if (arr[i] == 1) {
                printf("%d\n", lbound + i);
            }
        }
        free(arr);
        printf("Time: \n");
        for (int i = 0; i < size; i++) {
            printf("Estimated time: %4.6f\n", times[i]);
        }
    }
    MPI_Finalize();
    return 0;
}