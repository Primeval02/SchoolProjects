/*
Bradley Tate
lab1-3600
Refactored from C++ to C
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd;
    char name[100];
    write(1, "What is your name: ", 19);
    read(0, name, 100);
    write(1, name, strlen(name));
    fd = open("myfile.txt", O_CREAT | O_RDWR, S_IRWXU);
    write(fd, name, strlen(name));
    close(fd);
    
    return 0;
}
