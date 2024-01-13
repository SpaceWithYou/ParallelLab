#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void algo_lab1(int seed) {
	double start, end;
	int count = 10000000, max = -1, i = 0;
	int* Array = (int*)malloc(sizeof(int) * count);
	srand(seed);
	for (i = 0; i < count; i++) {
		Array[i] = rand() - 2 * rand();
	}
	start = omp_get_wtime();
	for (i = 0; i < count; i++)
	{
		if (Array[i] > max) {
			max = Array[i];
		}
	}
	end = omp_get_wtime();
	printf("max = %d, time = %3.7f\n", max, end - start);
	free(Array);
}

//Поиск максимального элемента в массиве первая лабораторная
void algo_lab1_p(int seed) {
	double start, end;
	int count = 10000000, max = -1, i = 0;
	int* Array = (int*)malloc(sizeof(int) * count);
	srand(seed);
	for (i = 0; i < count; i++) {
		Array[i] = rand() - 2 * rand();
	}
	start = omp_get_wtime();
	#pragma omp parallel for num_threads(8) schedule(guided, 1000000) //schedule(option)
	for (i = 0; i < count; i++) {
		if (Array[i] > max) {
			max = Array[i];
		}
	}
	end = omp_get_wtime();
	printf("Parallel max = %d, time = %3.7f\n", max, end - start);
	free(Array);
}

//Пример алгоритма с механизмом явной блокировки
//Поиск суммы элементов массива
void algo() {
	const int count = 10000;
	int* arr = (int*)malloc(sizeof(int) * count);
	long checksum = 0, sum = 0;
	for (int i = 0; i < count; i++) {
		arr[i] = rand();
		checksum += arr[i];
	}
	omp_lock_t lock;
	omp_init_lock(&lock);
	int i = 0;
	#pragma omp parallel num_threads(8)
	{
		long localsum = 0;
		#pragma omp for nowait
		for (i = 0; i < count; i++) {
			localsum += arr[i];
		}
		//Атомарно обновляем переменную
		omp_set_lock(&lock);
		sum += localsum;
		omp_unset_lock(&lock);
	}
	omp_destroy_lock(&lock);
	printf("Sum = %d, checksum = %d\n", sum, checksum);
}

int lab4(int argc, char** argv) {
	//200203 - OpenMP 2.0
	int month = _OPENMP % 100;
	printf("OpenMP date: %d month %d year, version: %1.1f\n", month, (_OPENMP - month) / 100, 2.0);
	printf("Available number of processors: %d, maximum available threads: %d\n", omp_get_num_procs(), omp_get_max_threads());
	printf("Current dynamic: %d\n", omp_get_dynamic());
	printf("Timer accuracy: %3.7f seconds\n", omp_get_wtick());
	//omp_get_nested()->OMP: Info #276: omp_get_nested routine deprecated, please use omp_get_max_active_levels instead.
	printf("Maximum nested parallel sections: %d\n", omp_get_max_active_levels());
	omp_sched_t kind;
	int chunk;
	omp_get_schedule(&kind, &chunk);
	printf("Shedule type: %d, chunk size: %d\n", kind, chunk);
	omp_set_max_active_levels(2);
	algo();
	printf("--------------\n");
	int seeds[10] = { 920215, 1234, 4321, 9999, 871345, 410813, 10000, 1999992, 7777777, 91375 };
	for (int i = 0; i < 10; i++) {
		//algo_lab1(seeds[i]);
		//algo_lab1_p(seeds[i]);
	}
}