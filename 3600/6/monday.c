#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//include for thread functions
#include <pthread.h>

void *mythread(void *arg)
{
    long int myID = (long int)(arg);
    int count = 20;
    while(1) {
        //printf("thread %li: Hello World!\n", myID);
        usleep(rand() % 10000 + 100);
    }
    return (void *)0;
}

int main(void)
{
    void *status;
    //printf("Hello World!\n");
    //declare a thread id
    pthread_t tid[4];
    //start the thread
    pthread_create(&tid[0], NULL, &mythread, (void *)0);
    pthread_create(&tid[1], NULL, &mythread, (void *)1);
    pthread_create(&tid[2], NULL, &mythread, (void *)2);
    //pthread_create(&tid[3], NULL, &mythread, (void *)3);
    pthread_join(tid[0], &status);
    pthread_join(tid[1], &status);
    pthread_join(tid[2], &status);
    //pthread_join(tid[3], &status);
    return 0;
}
