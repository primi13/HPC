// Computation of pi using the Leibniz formula
// gcc -fopenmp -o pil7 pil7.c
// srun --cpus-per-task=2 pil7

#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

#define N 100000000

int main(void) {
	double pi = 0.0;
	double *mysum = NULL;

	#pragma omp parallel master
	mysum = (double *)malloc(omp_get_num_threads() * sizeof(double));

	double startTime = omp_get_wtime();
	#pragma omp parallel
	{
		double piPart = 0.0;
		#pragma omp for
		for (int i = 0; i < N; i++) {
			int factor = 1 - 2 * (i % 2);
			piPart += 4.0 * factor / (2 * i + 1);
		}
		mysum[omp_get_thread_num()] = piPart;
	}
	for (int i = 0; i < omp_get_num_threads(); i++) {
		pi += mysum[i];
	}
	double endTime = omp_get_wtime();

	free(mysum);
	printf("pi: %lf, time taken: %lf seconds\n", pi, endTime - startTime);

	return 0;
}
