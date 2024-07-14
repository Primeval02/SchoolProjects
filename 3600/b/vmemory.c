#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main(void)
{
    system("w > data");
    int fd = open("data", O_RDONLY);
    if (fd < 0) {
        printf("error opening file.\n");
        exit(0);
    }
    int flen = lseek(fd, 0, SEEK_END); 
    //lseek(fd, 0, SEEK_SET);
    char* str = (char*)mmap(NULL, flen, PROT_READ, MAP_PRIVATE, fd, 0);
    //int x;
    //scanf("%i", &x);
    //char arr[100];
    //strncpy(arr, str+100, 25);
    //printf("%.10s\n", str);
    printf("%s\n", str);
    //printf("%s\n", arr);
    close(fd);
    return 0;
}
