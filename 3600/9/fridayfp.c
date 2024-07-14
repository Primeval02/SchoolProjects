#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int add(int a, int b) {return a + b;}
int sub(int a, int b) {return a - b;}
int mult(int a, int b) {return a * b;}
int divx(int a, int b) {return a / b;}


int main(void)
{
    srand((unsigned)time(NULL));
    int op = rand() % 4;
    int (*fptr)(int, int);
    printf("Enter 0, 1, 2, 3: ");
    scanf("%i", &op);

    if (op == 0) fptr = add;
    if (op == 1) fptr = sub;
    if (op == 2) fptr = mult;
    if (op == 3) fptr = divx;

    while (1) {
        printf("%i ", fptr(rand() % 9 + 1, rand() % 9 + 1));
    }
    return 0;
}
