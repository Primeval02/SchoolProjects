//Bradley Tate
//Monday 9/19/2022
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[], char* envp[])
{
    char** p = envp;

    while (*p != NULL) {
        printf("%s\n", *p);
        p++;
    }

    return 0;

    int pid = fork();
    if (pid == 0) {
        //child
        //execve("", , envp);
    } else {
        //parent
    }
    return 0;
}
