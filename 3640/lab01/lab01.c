#include <stdio.h>
#include <omp.h>

void main() {
	#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		printf("hello(%d) ", ID);
		printf(" world(%d) \n", ID);
	}
	
}
