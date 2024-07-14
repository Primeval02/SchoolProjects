/*  mine.c
 *
 *  This sample program is a little different than our other samples.
 *  The original author of this program was not Gordon.
 *  Look closely at how the sem grab/release operations are setup.
 *
 *  Introduction to using semaphores with threads 
 *  This code does not synchronize anything in particular,
 *  it just demonstrates how to use semaphores with threads,
 *  and how to setup semaphores using defines and variable names
 *  to aid in readability.
 *
 *  The program demonstrates how to bounce back and forth between 2 threads.
 *
 *     $ gcc -o mine mine.c -lpthread   # link in POSIX pthread library
 *     $ ./mine
 * OR
 *     make mine
 * OR
 *     make
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>

#define LIMIT 10
#define OTTER 0
#define SEAGULL 1

union semun {
	int     val;            
	struct  semid_ds *buf;  
	u_short *array;        
};

/* thread function prototype */
void seagull(void *dummy); 

/* threads share semaphores so semid needs to be global - all else is local */
int semid;

int main(void) {

	/*------------------------------------------------------------------*/
	/* Create two semaphores for producer/consumer synchronization      */
	/* Setup semaphore operations on the two shared semaphores.         */
	/* All of this must occur before creating the seagull thread.       */
	/* The seagull will have its own sembuf for sem operations.         */
	/*------------------------------------------------------------------*/

	char pathname[200];
	key_t ipckey;
	getcwd(pathname,200);
	strcat(pathname,"/foo");      /* foo must be in this directory */
	ipckey = ftok(pathname, 12);  /* 12 can be any unsigned int */ 
	int nsems = 2;                /* we need two semaphores for this problem */
	semid = semget(ipckey, nsems, 0666 | IPC_CREAT); /* 6 is read and write */
	if (semid < 0) {
		perror("semget:");
		_exit(1);
	}

	/* set initial value of sem 0 (first semaphore) in the set to 1 */
	if (semctl(semid, 0, SETVAL, 1) == -1) {
		fprintf(stdout,"error setting semaphore value\n");
	}
	/* set initial value of sem 1 (second semaphore) in the set to 0 */
	if (semctl(semid, 1, SETVAL, 0) == -1) {
		fprintf(stdout,"error setting semaphore value\n");
	}

	/* Create a buffer for two operations 
	 * these are operations from the
	 * perspective of the OTTER.
	   Be very careful to look at the sem_num on each operation.		
	*/
	struct sembuf mine[2];
	mine[0].sem_num = SEAGULL;   /* <---- seagull */
	mine[0].sem_flg = SEM_UNDO;
	mine[0].sem_op = 0;
	mine[1].sem_num = OTTER;     /* <---- otter   */
	mine[1].sem_flg = SEM_UNDO;
	mine[1].sem_op = -1;         /* <---- signal the otter */

	struct sembuf yours[1];
	yours[0].sem_num = SEAGULL; 
	yours[0].sem_flg = SEM_UNDO;
	yours[0].sem_op = +1;        /* seagull protects sem after grabbing it */

	/*---------------------------
	 * spawn the seagull thread * 
	 * the parent is the otter  *
	 *--------------------------*/
	pthread_t S;
	int dummy;
	if (pthread_create( &S, NULL,  (void *)seagull, (void *)&dummy) != 0)
		printf("Error creating thread\n");

	/* parent (otter) thread grabs fish */
	int count = 0;
	while (count < LIMIT) {
		semop(semid, mine, 2);
		count++;
		write(1,"yours ",6);
		semop(semid, yours, 1);
	}

	if (pthread_join(S, NULL) != 0)
		printf("Error rejoining from thread\n");

	/* cleanup semaphores */
	if ((semctl(semid, 0, IPC_RMID)) < 0) {
		perror("semctrl IPC_RMID");
		exit(EXIT_FAILURE);
	} else {
		puts("\nsemaphore removed");
		system("ipcs -s");
	} 

	return 0;
}

void seagull(void * dummy)
{
	/* setup semaphore operations for seagull */
	struct sembuf mine[2];  /* holds 2 operations */
	/*---*/
	mine[0].sem_num = OTTER;     /* <-------- otter   */
	mine[0].sem_flg = SEM_UNDO; 
	mine[1].sem_num = SEAGULL;   /* <-------- seagull */
	mine[1].sem_flg = SEM_UNDO;
	mine[0].sem_op = 0;          /* otter will wait on zero    */
	mine[1].sem_op = -1;         /* give otter the green light */
	struct sembuf yours[1]; 
	yours[0].sem_num = OTTER;  
	yours[0].sem_flg = SEM_UNDO; 
	yours[0].sem_op = +1;        /* otter protects sem after grabbing it */
	/*---*/
	int count = 0; 
	while (1) {  
		if (count >= LIMIT)
			break;
		semop(semid, mine, 2);
		++count;
		write(1, "mine ",5);
		semop(semid, yours, 1);
	}
	return;
}

