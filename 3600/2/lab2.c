/* Bradley Tate  9/1/2022 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/file.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSZ 100

void zeroOutBuf(char *str)
{
    for (int i = 0; i < BUFSZ; i++)
        str[i] = 0;
}

int fib(int n)
{
    if (n == 1 || n == 2)
        return 1;
    else 
        return fib(n-1) + fib(n-2);
}

int main(int argc, char *argv[])
{ 
    int n, status = 0; 
    /* If arguments take into n, otherwise exit */
    if (argc > 1) {
        n = atoi(argv[1]);
    }
    else {
        write(1, "Usage: lab2 n\n", 15);
        exit(1);
    }
    

    int cpid = fork();

    if (cpid == 0) {
        /* Child */
        char buf[BUFSZ];
        zeroOutBuf(buf);
        int logf;
        logf = open("log", O_WRONLY|O_CREAT|O_TRUNC, 0644);

        /* Writes time */
        time_t timer;
        struct tm *tm_info;
        time(&timer);
        tm_info = localtime(&timer);
        strftime(buf, 26, "%Y:%m:%d %H:%M:%S\n", tm_info);
        write(logf, buf, strlen(buf));

        /*Writes fib result */ 
        int ret = fib(n);
        zeroOutBuf(buf);
        sprintf(buf, "Fib sequence result: %d\n", ret);
        write(logf, buf, strlen(buf));

        close(logf);
        exit(n);
    } else {
        /* Parent */
        char buf[BUFSZ];
        wait(&status);
        zeroOutBuf(buf);
        if (WIFEXITED(status))
            sprintf(buf, "child killed and exited with code: %d\n", WEXITSTATUS(status));
        write(1, buf, strlen(buf));
        exit(0);
    }
    
	return 0;
}
