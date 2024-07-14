/*
Bradley Tate
lab1-3600
fork command
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int status;
    int pid = fork(); /*Creates a child or second process after fork() command*/
    if (pid == 0) {
        printf("I am the child\n");
    } else {
        printf("I am the parent  child pid: %i\n", pid);
        while(1) {

        }
        wait(&status);
    }
    printf("Hello\n");
    return 0;
}
