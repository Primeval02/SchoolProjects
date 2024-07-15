// Best runtimes
// -------------
// Serial: 0.0002051s
// Array of Locks: 0.004654s

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

int main() {
	int array[100000];
	int bins[10];
	int num, test = 0;
	srand(time(NULL));
	omp_lock_t locks[10];

	//Initializes locks
	for (int i = 0; i < 10; i++) {
		omp_init_lock(&locks[i]);
	}

	//Populates array w/ random ints 0-9
	for (int i = 0; i < 100000; i++) {
		num = rand() % 10;
		array[i] = num;
	}

	double startTime = omp_get_wtime(); // timer start

	//Array sampling

#pragma omp parallel
	{	
#pragma omp for reduction (+:bins) 
	for (int i = 0; i < 100000; i++) {
		test = array[i];

		omp_set_lock(&locks[test]);	
		bins[test] += 1;
		omp_unset_lock(&locks[test]);
	}
	}

	

	double runTime = omp_get_wtime() - startTime;

	//Results block
	std::cout << "Histogram bins: " << std::endl;
	for (int i = 0; i < 10; i++) {
		std::cout << "bin " << i << ": " << bins[i] << std::endl;
	}
	std::cout << "Sample runtime: " << runTime << " seconds" << std::endl;

	//Destroys locks
	for (int i = 0; i < 10; i++) {
		omp_destroy_lock(&locks[i]);
	}

	return 0;
}
