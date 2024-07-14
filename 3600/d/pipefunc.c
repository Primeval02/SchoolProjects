//Name: Bradley Tate, working w/ Jennifer Kuo
//Date: 11/17/22

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct t_data {
    int a;
    int b;
};

void pipeFunc(int* fd)
{
    pipe(fd);
}

void writeFunc(int writeEnd, struct t_data* data)
{ 
    write(writeEnd, data, sizeof(*data));
    return;
}

void readFunc(int readEnd, struct t_data* dstruct)
{
    read(readEnd, dstruct, sizeof(*dstruct));
    return;
}
