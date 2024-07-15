#include <stdio.h>
#include <omp.h>

#define MAX 1000
static int A[MAX];

void main() {
	double avg, startTime, runTime = 0.0;
	int i;

	startTime = omp_get_wtime();
#pragma omp parallel for reduction (+:avg)
	for(i = 0; i < MAX; i++) {
		avg += A[i];
	}
	avg = avg / MAX;
	runTime = omp_get_wtime() - startTime;
	printf("Time: %fs\n", runTime);
}	
