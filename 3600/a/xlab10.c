/*
 * dotprod.c
 *
 * This program exists in several other directories for cs3600, and the code
 * may be slightly different. It is just a sample program that you may
 * experiment with yourself.
 *
 *     $ gcc -o dotprod dotprod.c -lpthread
 *     $ ./dotprod
 *     80200         sum(i=1 to 400) i = 80200
 *  This threaded program computes the algebraic dot product of two vectors:
 *        a = <2,3,4>  and  b = <-1,3,5>
 *        _   _ 
 *        a . b = (2*-1)+(3*3)+(4*5) = -2+9+20 = 27.
 *
 *  A mutex enforces mutual exclusion on the shared structure: lock before 
 *  updating and unlock after updating.
 *
 *  The main program creates threads which do all the work and then print out 
 *  result upon completion. Before creating the threads, the input data is 
 *  created. The main thread needs to wait for all threads to complete, it 
 *  waits for each one of the threads. We specify a thread attribute value that 
 *  allow the main thread to join with the threads it creates. Note also that 
 *  we free up handles when they are no longer needed.
 *	 
 *  Each thread works on a different set of data. The offset is specified by 
 *  the loop variable 'i'. The size of the data for each thread is indicated
 *  by VECLEN.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
    int *a;
    int *b;
    /* int sum; */
    int veclen; 
} Dotdata;

#define NUM_THRDS 4
#define VEC_LEN 120
Dotdata dotstr;               /* global so all threads can see and use it */
pthread_t callThd[NUM_THRDS];
pthread_mutex_t mutexsum;     /* use a mutex to protect the dot product */

int pipefd[2];

void *dotprod(void *arg)
{
    /* thread function */
    int i, start, end, len ;
    long offset;
    int mysum, *x, *y;
    offset = (long)arg;

    len = dotstr.veclen;
    start = offset * len;
    end   = start + len;
    x = dotstr.a;
    y = dotstr.b;

    mysum = 0;
    for (i=start; i<end; i++) {
        mysum += (x[i] * y[i]);
    }
    pthread_mutex_lock (&mutexsum);
    /* dotstr.sum += mysum; */
    write(pipefd[1], &mysum, 4);

    pthread_mutex_unlock (&mutexsum);
    pthread_exit((void *)0);
}


int main (int argc, char *argv[])
{
    long i;
    int *a, *b;
    void *status;
    int sstatus;
    /*pthread_attr_t attr;*/
    pipe(pipefd);
    int pid = fork();
    if (pid == 0) {
        int in, sum = 0;
        close(pipefd[1]);
        while(read(pipefd[0], &in, 4) > 0)
            sum += in;
        printf("Sum = %d \n", sum);
        exit(EXIT_SUCCESS);
    } else {
        close(pipefd[0]);
        /* Assign storage and initialize values in the vectors */
        a = (int *)malloc(NUM_THRDS * VEC_LEN * sizeof(int));
        b = (int *)malloc(NUM_THRDS * VEC_LEN * sizeof(int));
        for (i=0; i<VEC_LEN * NUM_THRDS; i++) {
            a[i] = 1.0;
            b[i] = a[i];  /* over written in the next statement */
            b[i] = (i+1); /* integers from 1 to VECLEN*NUMTHRDS*/
        }

        dotstr.veclen = VEC_LEN; 
        dotstr.a = a; 
        dotstr.b = b; 
        /* dotstr.sum=0; */
        
        pthread_mutex_init(&mutexsum, NULL);

        /* create NUMTHRDS threads */
        for (i=0; i<NUM_THRDS; i++) {
            pthread_create(&callThd[i], NULL, dotprod, (void *)i);
        }

        /* wait on all the threads to finish */
        for (i=0; i<NUM_THRDS; i++) {
            pthread_join(callThd[i], &status);
        }

        /* now you can safely print out the results and cleanup */
        /* printf ("Sum =  %d \n", dotstr.sum); */
        if (a)
            free(a);
        if (b)
            free(b);
        close(pipefd[1]);
        wait(&sstatus);
        pthread_mutex_destroy(&mutexsum);
        printf("Child exited with code: %d\n", WEXITSTATUS(sstatus));
        exit(0);
    }
    return 0;
}

