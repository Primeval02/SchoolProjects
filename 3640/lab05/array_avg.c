#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

int main() {
	srand(time(NULL));
	int array[1000];
	int total = 0;
	int count = 0;
#pragma omp parallel
	{
#pragma omp master
		{
			for (int i = 0; i < 1000; i++) {
				int r = rand() % (1000 - 1) + 1;
				array[i] = r;
				total += r;
				count++;
			}

		}
	}
	int avg = total / 1000;
	printf("The avg of this array of 1000 ints is: %d\n", avg);
	printf("Master thread ran %d times\n", count);
}
