#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int algo1(int random_seed, int count, int *Array, FILE* f)
{
	int  max = -1;                  
	double time = 0;
	srand(random_seed);
    clock_t start = clock();
	for (int i = 0; i < count; i++)
	{
		if (Array[i] > max) { max = Array[i]; };
	}
    clock_t end = clock();
	time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("-- My lmax is: %d;\n", max);
	printf("algo's time elapsed = %3.6f\n", time);
	fprintf(f, "algo %3.6f \n", time);

	int ops = 0;
	max = -1;
	for (int i = 0; i < count; i++)
	{
		if (Array[i] > max) { max = Array[i];};
		ops++;
	}
	printf("-- My lmax is: %d;\n", max);
	printf("algo's Compares = %d\n", ops);
	//fprintf(f, "algo = %d\n", ops);
	fclose(f);
	return 0;
}