//Dining philosophers program
//Written in class Wednesday Oct 5, 2022
//Demonstrates deadlock and starvation.
//
//A small change was made after class to do the printf() of information
//as each philosopher is eating, and has not yet put down their forks.
//
//This program will deadlock. To fix it, make sure each philosopher
//picks up their lowest numbered fork first.
//
//Now starvation will be clearly seen.
//We will solve starvation in lab-7.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
sem_t xfork[5];
pthread_mutex_t monitor;
int eating[5] = {0,0,0,0,0};
const int neats = 30000;
int done = 0;

void *philosopher(void *arg)
{
	int my_number = (int)(long int)(arg);
	int my_forks[2] = { my_number, (my_number+1) % 5 };
	while (1) {
		if (done)
			break;
		//eat
        pthread_mutex_lock(&monitor);
		sem_wait(&xfork[my_forks[0]]);
		sem_wait(&xfork[my_forks[1]]);
        pthread_mutex_unlock(&monitor)
		eating[my_number]++;
		printf("%i %i %i %i %i\n",
				eating[0],eating[1],eating[2],eating[3],eating[4]);
		fflush(stdout);
		sem_post(&xfork[my_forks[1]]);
		sem_post(&xfork[my_forks[0]]);
		if (eating[my_number] >= neats)
			done = 1;
	}
	return (void *)0;
}

int main()
{
	int i;
	void *status;
    pthread_mutex_init(&monitor, NULL);
	pthread_t tid[5];
	for (i=0; i<5; i++)
		sem_init(&xfork[i], 0, 1);
	for (i=0; i<5; i++)
		pthread_create(&tid[i], NULL, philosopher, (void *)(long int)i);
	for (i=0; i<5; i++)
		pthread_join(tid[i], &status);
	for (i=0; i<5; i++)
		sem_destroy(&xfork[i]);
	return 0;
}



