/* Bradley Tate */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>

void sigusr_handler(int sig);

int status, logfd;
pid_t parent, cpid;

int main(void)
{
    struct sigaction sa;                  /* Setup handler */    
    sa.sa_handler = sigusr_handler; 
    sa.sa_flags = 0;

    sigset_t mask;                        /* Blocks all signals */
    sigfillset(&mask);                       
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigfillset(&sa.sa_mask);                /* Allow SIGUSR1 through sa mask */
    sigdelset(&sa.sa_mask, SIGUSR1);

if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    sigemptyset(&mask);                     /* Allow signals to be passed again */

    cpid = fork();

    if (cpid == 0) {
        logfd = open("log", O_WRONLY|O_CREAT|O_TRUNC, 0644);
        write(logfd, "Hello ", 6);
        sigsuspend(&sa.sa_mask);
        write(logfd, "Roadrunners\n", 12);
        close(logfd);
        exit(0);

    } else {
        kill(cpid, SIGTERM);                /* Pass the signals to the child */
        kill(cpid, SIGUSR1);
        wait(&status);
        printf("child terminated with exit code %d\n", WEXITSTATUS(status));
        exit(EXIT_SUCCESS);
    }
}

void sigusr_handler(int sig)
{   
    if (sig == SIGUSR1) {
        write(logfd, "got signal ", 11);
    }
}
