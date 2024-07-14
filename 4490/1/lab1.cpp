// Name: Bradley Tate
// Assignment: Lab 1
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <iostream>
#include <iomanip>

using namespace std;

int kbhit(void);

int main(void)
{
    puts("Keyboard Precision Game: Score between 1.0s and 1.2s to win.");
    puts("Press a key!");
    double time_taken = 0.0;
    char ch = ' ';
    int done = 0;
    int attempts = 0;
    clock_t start, end;
    while(!done) {
        start = clock();
        while(!kbhit()) {}
        ch = getchar();
        printf("You pressed '%c'!\n", ch);
        attempts++;
        end = clock();
        time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout << "Time since last key press: " << time_taken << setprecision(3) << "s." << endl;
        if (time_taken >= 1.000 && time_taken <= 1.200) {        // Right now end if time is between 1.000 and 1.5 since 1.000 is hard
            done = 1;
        }
    }
    printf("Time between the last two presses was between 1.0s and 1.2s, you win!\n");
    printf("Attempts taken: %i\n", attempts);
    return 0;
}

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}


