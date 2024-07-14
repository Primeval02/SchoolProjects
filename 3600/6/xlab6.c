/*
 *  echo.c
 *
 *  Demonstrate classic producer/consumer problem; the parent thread (the 
 *  producer) writes one character at a time from a file into a shared buffer
 *
 *  The buffer is shared because it is defined globally.
 *
 *  The consumer reads from buffer and writes it to a log file. With no 
 *  synchronization, the consumer will read the same character multiple times
 *  because the producer is slower than the consumer.
 *  Output is not deterministic - it varies across executions. 
 *
 *  This is a problem of concurrency. When two threads access shared resources 
 *  without synchronization it is a race condition as to which thread gets 
 *  there first or next. Without synchronization, the consumer may read a value
 *  multiple times (empty buffer) or the producer overwrite a value before the 
 *  consumer had a chance to read it. 
 *
 *    $ gcc -o echo echo.c -lpthread   # link in POSIX pthread library  
 *    $ ./echo 
 */

#include <pthread.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/sem.h>

#define DEBUG 0 
#define PRODUCER 1
#define CONSUMER 0

/* thread function prototypes */
void consumer(void *arg);
void producer(void *arg);

/* A note on the differences between forks and threads. Variables local to 
 * main that exist before the fork are inherited by the child but not shared.
 * Threads, since they are functions, can only see globals. These globals are
 * not only visible but shared by all threads since threads share user space.
 */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
} my_semun;

int n_waiting;
int sem_value;
pid_t lastpid;
key_t ipckey;
int semid;
int nsem = 2;

int retval;
int LIMIT;  /* for testing read 55 chars from the file */
char buf[1];           /* 1 char buffer */
int fib(int);
FILE *fin;
FILE *fout;

int main(int argc, char *argv[])
{
    if(argc >= 2)
        LIMIT = atoi(argv[1]);
    else
        LIMIT = 50;
    

    pthread_t cthr, pthr;  /* cthr will point to the consumer thread */
    int dummy;

    semid = semget(ipckey, nsem, 666 | IPC_CREAT);
    
    semctl(semid, 0, SETVAL, 0);
    semctl(semid, 1, SETVAL, 1);

    /* we are using formatted fopen(2) to make life easier - normally
     * in systems coding you would use open(2)
     */
    fin = fopen("poem", "r");
    if (!fin) {
        fprintf(stderr, "error opening input file.\n");
        exit(1);
    }

    fout = fopen("log", "w");
    if (fout == NULL) {
        fprintf(stderr, "error opening output file.\n");
        exit(1);
    }

    /* create consumer thread */
    if (pthread_create(&cthr, NULL,  (void *)consumer, (void *)&dummy) != 0)
        fprintf(stderr,"Error creating thread\n");

    /* PRODUCER
     * reads 1 char from poem file and writes that char to buffer */
    if (pthread_create(&pthr, NULL,  (void *)producer, (void *)&dummy) != 0)
        fprintf(stderr,"Error creating thread\n");

    /* the parent thread always joins with its spawned threads */
    if ((pthread_join(cthr, (void*)&retval)) < 0) {
        perror("pthread_join");
    } else {
        if (DEBUG)
            printf("joined consumer thread w exit code: %d\n", retval);
    }
    /* parent closes input file */
    fclose(fin);
    fclose(fout);
    semctl(semid, nsem, IPC_RMID);
    exit(0);
}

/* CONSUMER thread function
 * reads 1 char from shared buffer and writes char to screen
 */
void consumer(void *arg)
{
    struct sembuf grab[2];
    grab[0].sem_num = PRODUCER;
    grab[0].sem_flg = SEM_UNDO;
    grab[1].sem_num = CONSUMER;
    grab[1].sem_flg = SEM_UNDO;
    grab[0].sem_op = 0;
    grab[1].sem_op = -1;

    struct sembuf release[1];
    release[0].sem_num = PRODUCER;
    release[0].sem_flg = SEM_UNDO;
    release[0].sem_op = +1;

    int count = 0;
    pid_t tid = syscall(SYS_gettid);
    fprintf(fout, "consumer thread pid: %d tid: %d \n", getpid(), tid);
    while (1) {
        if (count == LIMIT)
            break;
        semop(semid, grab, 2);
        fputc(buf[0], fout);
        fib(15);   /* make consumer slightly faster than producer */
        count++;
        semop(semid, release, 1);
    }
    fputc('\n', fout);
    pthread_exit(0);
}

void producer(void *arg)
{
    struct sembuf grab[2];
    grab[0].sem_num = CONSUMER;
    grab[0].sem_flg = SEM_UNDO;
    grab[0].sem_op = 0;
    grab[1].sem_num = PRODUCER;
    grab[1].sem_flg = SEM_UNDO;
    grab[1].sem_op = -1;

    struct sembuf release [1];
    release[0].sem_num = CONSUMER;
    release[0].sem_flg = SEM_UNDO;
    release[0].sem_op = +1;
    int count = 0;
    buf[0] = ' ';
    pid_t tid = syscall(SYS_gettid);
    fprintf(fout, "producer thread pid: %d tid: %d \n", getpid(), tid);
    while (1) {
        if (count == LIMIT)
            break;
        semop(semid, grab, 2);
        /* critical code */
        buf[0] = fgetc(fin);
        if (DEBUG)
            putc(buf[0], fout);
        fib(16);    /* make the producer slightly slower than consumer */
        count++;
        semop(semid, release, 1);

    }
}

int fib(int n)
{
    if (n == 1 || n == 2)
        return 1;
    return fib(n-1) + fib(n-2);
}


