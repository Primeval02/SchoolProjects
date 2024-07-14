//Group: Bradley Tate, working w/ Jennifer Kuo
//Date: 11/17/22

#include <stdio.h>
//#include <stdlib.h>
//#include <fcntl.h>
//#include <unistd.h>

int main()
{
    struct t_data {
        int a;
        char c;
    } data;
    
    extern void pipeFunc(int* fd);
    extern void writeFunc(int writeEnd, struct t_data* data);
    extern void readFunc(int readEnd, struct t_data* dstruct);


    printf("Size of my structure: %lu\n", sizeof(data));
    data.a = 1234;
    data.c = 'A';
    //char buf[100];
    int fd[2];

    //pipe(fd);
    pipeFunc(fd);

    //char c = 'a';
    //int num = 12345;
    
    //write(fd[1], &data, sizeof(data));
    writeFunc(fd[1], &data);

    //int dest;
    struct t_data dstruct;

    //read(fd[0], &dstruct, sizeof(data));
    readFunc(fd[0], &dstruct);

    printf("values from pipe: %i and %c\n", dstruct.a, dstruct.c);

    return 0;
}
