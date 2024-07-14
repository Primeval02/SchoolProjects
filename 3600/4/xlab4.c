/* Bradley Tate 9/15/2022 */
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    int status, num;

    char buf[1024];
    
    struct {
        long type;
        char text[100];
    } mymsg;

    /* Create IPC key */
    char pathname[128];
    getcwd(pathname, 128);
    strcat(pathname, "/foo");
    int ipckey = ftok(pathname, 21);

    /* Open log file */
    int logfd;
    logfd = open("log", O_WRONLY|O_CREAT|O_TRUNC, 0644);

    /* Create shared memory and message queue */
    int shmid;
    shmid = shmget(ipckey, sizeof(int), IPC_CREAT | 0666);
    int mqid = msgget(ipckey, IPC_CREAT | 0666);

    int *shared = shmat(shmid, (void *) 0, 0);

    pid_t child = fork();

    if (child == 0) {

        /* Busy loop, waits for change inshared memory */
        while (1) {
            if (*shared > 0)
                break;
            usleep(1000);
        }

        /* Writes the number and passed message */
        msgrcv(mqid, &mymsg, sizeof(mymsg), 0, 0);
        sprintf(buf, "%d\n", *shared);
        write(logfd, buf, strlen(buf));
        sprintf(buf, "%s", mymsg.text);
        write(logfd, buf, strlen(buf)); 
        close(logfd);
        shmdt(shared);
        exit(0);
    } else {
        /* Asks for an int and sets shared memory to int */
        write (1, "Input a 2 digit number: ", 24);
        memset(buf, 0, 1024);
        read(0, buf, 3);
        num = atoi(buf);
        *shared = num;
        /* Asks for and sends word */
        write (1, "\nInput a word: ", 15);
        memset(buf, 0, 1024);
        read(0, buf, 50);
        strcpy(mymsg.text, buf);
        mymsg.type = 1; 
        msgsnd(mqid, &mymsg, sizeof(mymsg), 0);
        wait(&status);
        printf("child exited with code %d\n", WEXITSTATUS(status));
        shmdt(shared);
        shmctl(shmid, IPC_RMID, 0);
        msgctl(mqid, IPC_RMID, NULL);
    }
    return 0;
}
