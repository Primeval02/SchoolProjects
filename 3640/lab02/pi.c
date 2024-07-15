/*

   This program will numerically compute the integral of

   4/(1+x*x) 

   from 0 to 1.  The value of this integral is pi -- which 
   is great since it gives us an easy way to check the answer.

   The is the original sequential program.  It uses the timer
   from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.

 */
#include <stdio.h>
#include <omp.h>
static long num_steps = 100000000;
double step;
int main ()
{
	double x, pi, finalSum = 0.0;
	double start_time, run_time;

	step = 1.0/(double) num_steps;  //step size


	start_time = omp_get_wtime();// this is the timer we want to use when using OpemMP

	omp_set_num_threads(4); // allocate # of threads

	#pragma omp parallel // give each thread their own section of curve to calculate & sum at end 
	{
		int cID = omp_get_thread_num(); // declare here to give each thread these variables
		int i;
		double x, sum = 0.0;
		if (cID == 0) {
			for (i = 1; i <= num_steps / 4; i++){ // goes through each rectangle on the curve and sums them
				x = (i + 0.5) * step;
				sum += 4.0 / (1.0 + x * x);
			}
			printf("Thread 0 sum: %f ", sum*step);
		}
		
		if (cID == 1) {
			for (i = num_steps / 4; i <= num_steps * 1 / 2; i++){ // goes through each rectangle on the curve and sums them
				x = (i - 0.5) * step;
				sum = sum + 4.0 / (1.0 + x * x);
			}
			printf("Thread 1 sum: %f ", sum*step);
		}

		if (cID == 2) {
			for (i = num_steps * 1 / 2; i <= num_steps * 3 / 4; i++){ // goes through each rectangle on the curve and sums them
				x = (i - 0.5) * step;
				sum = sum + 4.0 / (1.0 + x * x);
			}
			printf("Thread 2 sum: %f ", sum*step);
		}

		if (cID == 3) {
			for (i = num_steps * 3 / 4; i <= num_steps; i++){ // goes through each rectangle on the curve and sums them
				x = (i - 0.5) * step;
				sum = sum + 4.0 / (1.0 + x * x);
			}
			printf("Thread 3 sum: %f ", sum*step);
		}

		finalSum += sum;
	}

	pi = step * finalSum; // calculates answer by mult the sum of all steps by the step size
	run_time = omp_get_wtime() - start_time; // calculates runtime current - time at start
	printf("\n pi with %ld steps is %lf in %lf seconds\n ", num_steps, pi, run_time);
}	  





