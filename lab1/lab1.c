#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int lab1(int argc, char** argv)
{
    int count = 10000000;           ///< Number of array elements
    int threads = 16;               ///< Number of parallel threads to use
    int seeds[10] = {920215, 1234, 4321, 9999, 871345, 410813, 10000, 1999992, 7777777, 91375}; ///<Random seeds
    int* Array = 0;                 ///< The array we need to find the max in
    int  max = -1;                  ///< The maximal element
    double start = 0, end = 0, time_start = 0, time_end = 0, time = 0;
    printf("OpenMP: %d;\n======\n", _OPENMP);
    FILE* f = fopen("results_task.txt", "w");
    fclose(f);
    /* Generate the random array */
    Array = (int*)malloc(count * sizeof(int));
    for (int k = 0; k < 10; k++)
    {
        f = fopen("results_task.txt", "a+");
        fprintf(f, "-------------------\n");
        fclose(f);
        for (int j = 16; j > 0; j--)
        {
            threads = j;
            start = 0, end = 0, time_start = 0, time_end = 0, time = 0;
            srand(seeds[k]);
            for (int i = 0; i < count; i++) 
                Array[i] = rand();
            int s;
            
            time_start = omp_get_wtime();
            #pragma omp parallel num_threads(threads) shared(Array, count) reduction(max: max) default(none) reduction(+: start, end)
            {
                start = omp_get_wtime();
                #pragma omp for
                for (s = 0; s < count; s++)
                {
                    if (Array[s] > max) { max = Array[s]; };
                }
                end = omp_get_wtime();
                printf("-- My lmax is: %d; thread_num = %d\n", max, omp_get_thread_num());
            }
            time_end = omp_get_wtime();
            time = (end - start) / j;               ///< Average working time of thread
            time /= (time_end - time_start);        ///< Amount of work
            f = fopen("results_task.txt", "a+");
            printf("======\nMax is: %d;\n", max);
            printf("threads = %d, wasted_time = %3.2f;\n", j, (1 - time) * 100);    ///< Wasted work
            fprintf(f, "%3.6f \n", (1 - time) * 100);
            max = -1;
            fclose(f);
        }
    }
    return(0);
}