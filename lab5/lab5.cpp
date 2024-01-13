#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <omp.h>

int lab5(int argc, char** argv)
{
	int ret = -1;	///< For return values
	int size = -1;	///< Total number of processors
	int rank = -1;	///< This processor's number

	const int count = 100000000; ///< Number of array elements

	int* array = 0; ///< The array we need to find the max in
	int lmax = -1;	///< Local maximums
	int  max = -1;  ///< The maximal element
	double start, end;

	const int random_seed = atoi(argv[1]); ///< RNG seed
	/* Initialize the MPI */
	ret = MPI_Init(&argc, &argv);

	/* Determine our rank and processor count */
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/* Allocate the array */
	array = (int*)malloc(count * sizeof(int));
	if (!rank) {
		srand(random_seed);
		for (int i = 0; i < count; i++) { array[i] = rand(); }
	}

	start = MPI_Wtime();
	MPI_Bcast(array, count, MPI_INTEGER, 0, MPI_COMM_WORLD);

	const int wstart = (rank)*count / size;
	const int wend = (rank + 1) * count / size;

	for (int i = wstart; i < wend; i++) {
		if (array[i] > lmax) {
			lmax = array[i];
		}
	}

	MPI_Reduce(&lmax, &max, 1, MPI_INTEGER, MPI_MAX, 0, MPI_COMM_WORLD);
	end = MPI_Wtime();
	ret = MPI_Finalize();
	if (!rank) {
		printf("%g\n", end - start);
	}
	return(0);
}

/*
	const int count = 100000000;
	int* Array = (int*)malloc(count * sizeof(int));
	int max = -1;
	int seeds[10] = { 920215, 1234, 4321, 9999, 871345, 410813, 10000, 1999992, 7777777, 91375 };
	double start, end;
	//for (int j = 0; j < 10; j++) {
	//	max = -1;
	//	srand(seeds[j]);
	//	double start = omp_get_wtime();
	//	for (int i = 0; i < count; i++) {
	//		Array[i] = rand();
	//	}
	//	for (int i = 0; i < count; i++)
	//	{
	//		if (Array[i] > max) { max = Array[i]; };
	//	}
	//	double end = omp_get_wtime();
	//	printf("algo's time elapsed = %3.6f\n", ((double)(end - start)) / CLOCKS_PER_SEC);
	//}
	//return 0;
	for (int k = 0; k < 10; k++) {
		max = -1;
		srand(seeds[k]);
		for (int i = 0; i < count; i++) {
			Array[i] = rand();
		}
		for (int j = 16; j > 0; j--) {
			printf("Threads = %d\n", j);
			max = -1;
			start = omp_get_wtime();
			#pragma omp parallel for num_threads(j) shared(Array, count)
			for (int i = 0; i < count; i++) {
				if (Array[i] > max) { max = Array[i]; }
			}
			end = omp_get_wtime();
			printf("time = %g\n", end - start);
		}
	}
*/