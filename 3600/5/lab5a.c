/*
 * lab5.c
 *
 * You may start with this program for lab-5
 * 
 * This file uses shared memory. If you do not understand how shared memory 
 * works, review week-4 examples. Your job in lab-5 is to add a semaphore 
 * to control order. You know things have gone awry when you run the code:
 *  
 *    $ make lab5
 *    $ ./lab5
 *    child reads: 0
 *
 * The desired result is for the parent to compute fib(n), write the result to
 * shared memory then the child reads the result and displays it to the screen.
 * The problem is that things are out of order - by the time the parent computes
 * fib(10) the child has already read memory; i.e., the parent modifies the 
 * segment too late.
 * 
 * This scenario is a race condition. For example, if you pass a small enough 
 * number to fib, the child generally grabs the value OK:
 *
 *    $ ./lab5 10
 *    child reads: 55 
 *
 * but this may not work
 *
 *    $ ./lab5 18
 *    child reads: 0  <---- wrong results 
 *
 * To fix this problem you need to add a semaphore to control order. You want 
 * the parent to grab the semaphore first. Meanwhile the child is blocked on 
 * the semaphore. After the parent writes fib(n) to memory the parent releases 
 * the semaphore and the child can then grab it.
 * 
 */

#include <sys/types.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* POSIX semaphores */
#include <semaphore.h>
sem_t *sem1, *sem2;


#define BUFSIZE 256
int status;
int fib(int);

int main(int argc, char **argv)
{
	int n = 20;
	char buf[BUFSIZE];
	pid_t cpid;
	int shmid; 
	int *shared;

	/* n was entered on command line */
	if (argc >= 2)
		n = atoi(argv[1]);

    /* define and initialize semaphores */
    sem1 = sem_open("/btate1", O_CREAT, 0644, 0);    
    sem2 = sem_open("/btate2", O_CREAT, 0644, 1);

	/* IPC_PRIVATE will provide a unique key without using an ipckey 
	 * it works with related processes but not unrelated ones - it is
	 * a safe way to get a ipckey to use in a fork */
	shmid = shmget(IPC_PRIVATE, sizeof(int)*100, IPC_CREAT | 0666);
	/* attach and initialize memory segment */
	shared = shmat(shmid, (void *) 0, 0);
	*shared = 0;

	/* Just above we created some shared memory.  */
	/* Enough to hold 100 4-byte integers.        */

	cpid = fork();

	if (cpid == 0) { 
		/* CHILD */
		/* Attach to shared memory -
		 * both child and parent must do this
		 * but the parent can do it before the fork */
		shared = shmat(shmid, (void *) 0, 0);

        /* child must wait for a semaphore */
        sem_wait(sem1);

		/* child reads and displays shared memory */
		int val = *shared;
		sprintf(buf, "child reads: %d\n", val);
		write(1, buf, strlen(buf));

		shmdt(shared); /* detach from segment */
		exit(0);
	} else {
		/* PARENT */

        /* child must wait for a semaphore */
        //sem_wait(sem2);

		/* Parent computes fib(n) and writes it to shared memory */
		*shared = fib(n);

        /* release the child semaphore */
        sem_post(sem1);

		wait(&status); 

		shmdt(shared);              /* detach from segment   */
		shmctl(shmid, IPC_RMID, 0); /* remove shared segment */
        sem_close(sem1);
        sem_close(sem2);
        sem_unlink("/btate1");
        sem_unlink("/btate2");
        sem_destroy(sem1);
        sem_destroy(sem2);
	}
	return 0;
}

/* Some busy work for the parent */
int fib(int n)
{
	/* We will write this function in class together. */
    if (n <= 2)
        return 1;
    return fib(n - 1) + fib(n - 2);
}



