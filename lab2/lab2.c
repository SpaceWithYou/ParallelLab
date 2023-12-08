#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int lab2(int argc, char** argv)
{
    int count = 10000000;           ///< Number of array elements
    int random_seed[10] = {920214, 1234, 4321, 9999, 871345, 410813, 10000, 1999992, 7777777, 91375 }; ///< RNG seeds
    //const int target = 123999;    ///< Number to look for
    const int target = 325;         ///< Number to look for
    int* Array = 0;                 ///< The array we need to find the max in
    int index = -1;                 ///< The index of the element we need
    int i = 0;
    double start = 0, end = 0;
    Array = (int*)malloc(count * sizeof(int));
    FILE* file = fopen("text.txt", "a+");
    /* Initialize the RNG */
    for (int k = 0; k < 10; k++) 
    {
        index = -1;
        count = 10000000;
        srand(random_seed[k]);
        /* Generate the random array */
        for (int i = 0; i < count; i++) { Array[i] = rand(); }
        /* Find the index of the element */
        start = omp_get_wtime();
        for (i = 0; i < count; i++)
        {
            if (Array[i] == target)
            {
                index = i;
                break;
            }
        }
        end = omp_get_wtime();
        printf("index = %d, time = %3.7f\n", index, end - start);
        fprintf(file,  "Original:%3.7f\n", end - start);
        
        i = 0;
        index = -1;
        /* Find the index of the element */
        start = omp_get_wtime();
        #pragma omp parallel num_threads(8) shared(index, count) default(none)  
        {
            #pragma omp parallel for
            for (i = 0; i < count; i++)
            {
                if (Array[i] == target)
                {
                    index = i;
                    count = -1;
                    break;
                }
            }
        }
        end = omp_get_wtime();
        printf("index = %d, time = %3.7f\n", index, end - start);
        fprintf(file, "Parallel:%3.7f\n", end - start);
    }
    fprintf(file, "==============\n");
    fclose(file);
    return(0);
}