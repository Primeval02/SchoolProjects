//Bradley Tate

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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
sem_t xfork[5];
pthread_mutex_t monitor;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
} my_semun;

key_t ipckey;
int semid;
int nsem = 5;
int sem_value;

struct sembuf grab[5][2], release[5][1];
int eating[5] = {0,0,0,0,0};
int neats; 
int fibnum;
int done = 0;
int fib(int n);

void *philosopher(void *arg)
{

    int my_number = (int)(long int)(arg);
    int my_forks[2] = { my_number, (my_number+1) % 5 };

    while (1) {
        if (done)
            break;
        //eat
        fib(fibnum);
        pthread_mutex_lock(&monitor);
        //sem_wait(&xfork[my_forks[0]]);
        //sem_wait(&xfork[my_forks[1]]);
        semop(semid, grab[my_forks[0]], 2);
        semop(semid, grab[my_forks[1]], 2);
        pthread_mutex_unlock(&monitor);
        eating[my_number]++;
        printf("%i %i %i %i %i\n",
                eating[0],eating[1],eating[2],eating[3],eating[4]);
        fflush(stdout);
        //sem_post(&xfork[my_forks[1]]);
        //sem_post(&xfork[my_forks[0]]);
        semop(semid, release[my_forks[0]], 1);
        semop(semid, release[my_forks[1]], 1);
        if (eating[my_number] >= neats)
            done = 1;
    }
    return (void *)0;
}

int fib(int n)
{
    if (n <= 2)
        return 1;
    return fib(n - 1) + fib(n - 2);
}

int main(int argc, char *argv[])
{
    int i;
    void *status;
    pthread_t tid[5];
    pthread_mutex_init(&monitor, NULL);

    semid = semget(ipckey, nsem, 666 | IPC_CREAT);


    for (int i = 0; i < 5; i++)
        semctl(semid, i, SETVAL, 0);

    //struct sembuf grab[5][2], release[5][1];
    for (int i = 0; i < 5; i++) {
        grab[i][0].sem_num = i;
        grab[i][0].sem_flg = SEM_UNDO;
        grab[i][0].sem_op = 0;
        grab[i][1].sem_num = i;
        grab[i][1].sem_flg = SEM_UNDO;
        grab[i][1].sem_op = 1;

        release[i][0].sem_num = i;
        release[i][0].sem_flg = SEM_UNDO;
        release[i][0].sem_op = -1;
    }

    fibnum = 10;
    neats = 10;
    if (argc == 3) { 
        fibnum = atoi(argv[1]);
        neats = atoi(argv[2]);
    }

    for (i=0; i<5; i++)
        pthread_create(&tid[i], NULL, philosopher, (void *)(long int)i);
    for (i=0; i<5; i++)
        pthread_join(tid[i], &status);
    
    semctl(semid, nsem, IPC_RMID);
    return 0;
}



