#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    struct t_data {
        int a;
        char c;
    } data;

    printf("Size of my structure: %lu\n", sizeof(data));
    data.a = 1234;
    data.c = 'A';
    //char buf[100];
    int fd[2];
    pipe(fd);
    //char c = 'a';
    //int num = 12345;
    write(fd[1], &data, sizeof(data));
    //int dest;
    struct t_data dstruct;
    read(fd[0], &dstruct, sizeof(data));
    printf("values from pipe: %i and %c\n", dstruct.a, dstruct.c);

    return 0;
}
