#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

//Заполним массив D числами Фибоначчи
//Время заполнения массива D не учитываем,
//т.к. является предподсчетом
int *makeD(int count, int size) {
	int* D = (int*)malloc(size * sizeof(int));
	D[size - 1] = 1;
	D[size - 2] = 2;
	for (int i = size - 3; i >= 0; i--) {
		D[i] = D[i + 1] + D[i + 2];
	}
	return D;
}

//Сортировка Шелла(последовательная)
void ShellSort(int Array[], int count, int D[], int d_size) {
	int d = 0, temp, k = 0;
	for (int i = 0; i < d_size; i++) {
		d = D[i];
		for (int j = d; j < count; j++) {
			temp = Array[j];
			k = j;
			for (; k >= d && Array[k - d] > temp; k -= d) {
				Array[k] = Array[k - d];
			}
			Array[k] = temp;
		}
	}
}

//Сортировка Шелла(параллельная)
//Выполняем параллельно Сортировку Шелла
//Делим массив на подсписки, состоящие из элементов, находящихся на расстоянии d_i
//Параллельно их сортируем, затем сливаем в один массив и увеличиваем i
//До тех пор пока не дойдем до последнего d_i = 1
//Тогда будет обычная сортировка вставками
void ShellSortParallel(int Array[], int count, int D[], int d_size) {
	int d = 0, temp, i, j, k, m = 0;
	for (i = 0; i < d_size; i++) {
		d = D[i];
#pragma omp parallel num_threads(8) shared(Array, d, count, d_size, D, i) private(temp, k, j, m) default(none)
		{
			#pragma omp for private(temp, k, j) schedule(static)
			for (j = 0; j < d; j++) {
				for (m = j; m < count; m += d) {
					temp = Array[m];
					k = m - d;
					while (k >= j && Array[k] > temp) {
						Array[k + d] = Array[k];
						k -= d;
					}
					Array[k + d] = temp;
				}
			}
		}
	}
}

//Функция для проверки равенства массивов
//Сравнивается массив Array c эталонным SampleArray
bool check(int Array[], int SampleArray[], int n) {
	for (int i = 0; i < n; i++) {
		if (Array[i] != SampleArray[i]) {
			printf("distinct in %d, Sample = %d, Arr = %d\n", i, SampleArray[i], Array[i]);
			return false;
		}
	}
	return true;
}

//Функция подсчета различных элементов массива (на входе отсортированный массив)
int RepeatsInArray(int Array[], int n) {
	int count = 1, element = Array[0];
	for (int i = 0; i < n; i++) {
		if (Array[i] != element) {
			element = Array[i];
			count++;
		}
	}
	return count;
}

//Запуск алгоритмов(последовательного и параллельного) на массивах с прямым порядком
//и с подмассивами с прямым порядком (все числа различные)
void DirectOrderTests(int Array1[], int Array2[], int count, int D[], int d_size) {
	double start, end;
	for (int i = 0; i < count; i++) {
		Array1[i] = i;
		Array2[i] = i;
	}
	printf("Full sorted array\n");
	start = omp_get_wtime();
	ShellSort(Array1, count, D, d_size);
	end = omp_get_wtime();
	printf("Original algorithm estimated time = %3.6f\n", end - start);
	start = omp_get_wtime();
	ShellSortParallel(Array2, count, D, d_size);
	end = omp_get_wtime();
	printf("Parallel algorithm estimated time = %3.6f\n", end - start);
	if(check(Array2, Array1, count)) {
		printf("OK\n");
	}
	for (int i = 0; i < count / 3; i++) {
		Array1[i] = count / 3 - i;
		Array2[i] = count / 3 - i;
	}
	for (int i = count / 3; i < count; i++) {
		Array1[i] = i;
		Array2[i] = i;
	}
	printf("Part sorted array\n");
	start = omp_get_wtime();
	ShellSort(Array1, count, D, d_size);
	end = omp_get_wtime();
	printf("Original algorithm estimated time = %3.6f\n", end - start);
	start = omp_get_wtime();
	ShellSortParallel(Array2, count, D, d_size);
	end = omp_get_wtime();
	printf("Parallel algorithm estimated time = %3.6f\n", end - start);
	if (check(Array2, Array1, count)) {
		printf("OK\n");
	}
}

//Запуск алгоритмов(последовательного и параллельного) на массивах с обратным порядком
//и с подмассивами с обратным порядком (все числа различные)
void ReverseOrderTests(int Array1[], int Array2[], int count, int D[], int d_size) {
	double start, end;
	for (int i = 0; i < count; i++) {
		Array1[i] = count - i;
		Array2[i] = count - i;
	}
	printf("Reverse array\n");
	start = omp_get_wtime();
	ShellSort(Array1, count, D, d_size);
	end = omp_get_wtime();
	printf("Original algorithm estimated time = %3.6f\n", end - start);
	start = omp_get_wtime();
	ShellSortParallel(Array2, count, D, d_size);
	end = omp_get_wtime();
	printf("Parallel algorithm estimated time = %3.6f\n", end - start);
	if (check(Array2, Array1, count)) {
		printf("OK\n");
	}
	for (int i = 0; i < count / 3; i++) {
		Array1[i] = i;
		Array2[i] = i;
	}
	for (int i = count / 3; i < count; i++) {
		Array1[i] = count - i;
		Array2[i] = count - i;
	}
	printf("Part reversed array\n");
	start = omp_get_wtime();
	ShellSort(Array1, count, D, d_size);
	end = omp_get_wtime();
	printf("Original algorithm estimated time = %3.6f\n", end - start);
	start = omp_get_wtime();
	ShellSortParallel(Array2, count, D, d_size);
	end = omp_get_wtime();
	printf("Parallel algorithm estimated time = %3.6f\n", end - start);
	if (check(Array2, Array1, count)) {
		printf("OK\n");
	}
}

//Запуск алгоритмов(последовательного и параллельного) на массивах с большим числом одиннаковых значений
void RepeatsOrderTests(int Array1[], int Array2[], int count, int D[], int d_size) {
	double start, end;
	for (int i = 0; i < count; i++) {
		Array1[i] = 1;
		Array2[i] = 1;
	}
	printf("Repetitions array\n");
	start = omp_get_wtime();
	ShellSort(Array1, count, D, d_size);
	end = omp_get_wtime();
	printf("Original algorithm estimated time = %3.6f\n", end - start);
	start = omp_get_wtime();
	ShellSortParallel(Array2, count, D, d_size);
	end = omp_get_wtime();
	printf("Parallel algorithm estimated time = %3.6f\n", end - start);
	if (check(Array2, Array1, count)) {
		printf("OK\n");
	}
	for (int i = 0; i < count; i += count / 10) {
		for (int j = 0; j < count / 10; j++) {
			Array1[i + j] = i;
			Array2[i + j] = i;
		}
	}
	printf("Part Repetitions array\n");
	start = omp_get_wtime();
	ShellSort(Array1, count, D, d_size);
	end = omp_get_wtime();
	printf("Original algorithm estimated time = %3.6f\n", end - start);
	start = omp_get_wtime();
	ShellSortParallel(Array2, count, D, d_size);
	end = omp_get_wtime();
	printf("Parallel algorithm estimated time = %3.6f\n", end - start);
	if (check(Array2, Array1, count)) {
		printf("OK\n");
	}
}

//Запуск алгоритмов(последовательного и параллельного) на массивах с произвольными данными
void RandomTests(int Array1[], int Array2[], int count, int D[], int d_size, int seed) {
	double start, end;
	srand(seed);
	for (int i = 0; i < count; i++) {
		Array1[i] = rand();
		Array2[i] = Array1[i];
	}
	printf("Random array\n");
	start = omp_get_wtime();
	ShellSort(Array1, count, D, d_size);
	end = omp_get_wtime();
	printf("Original algorithm estimated time = %3.6f\n", end - start);
	start = omp_get_wtime();
	ShellSortParallel(Array2, count, D, d_size);
	end = omp_get_wtime();
	printf("Parallel algorithm estimated time = %3.6f\n", end - start);
	if (check(Array2, Array1, count)) {
		printf("OK\n");
		int elements = RepeatsInArray(Array1, count);
		printf("%d distinct elements %3.2f%%\n", elements, (elements + 0.0) / (count + 0.0) * 100);
	}
}

//Сортировка Шелла в порядке неубывания
int lab3(int argc, char** argv) {
	const int count = 100000;
	const int d_size = 20;
	int* Array1 = (int*)malloc(count * sizeof(int));
	int* Array2 = (int*)malloc(count * sizeof(int));
	int* D = makeD(count, d_size);
	DirectOrderTests(Array1, Array2, count, D, d_size);
	ReverseOrderTests(Array1, Array2, count, D, d_size);
	RepeatsOrderTests(Array1, Array2, count, D, d_size);
	RandomTests(Array1, Array2, count, D, d_size, 12345);
	return 0;
}