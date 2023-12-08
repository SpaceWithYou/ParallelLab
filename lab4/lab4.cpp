#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

//Пример алгоритма с механизмом явной блокировки
void algo() {
	//
}

int main(int argc, char** argv) {
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
}