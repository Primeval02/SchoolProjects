#include <iostream>
#include <cstring>

using namespace std;

void interpret(char input[], char killword[]);

int main()
{
    char input[100];
    char killword[4] = "Bye";

    while (strcmp(input, killword) != 0)
    {
        cout << "> ";
        cin.getline(input, 50);
        interpret(input, killword);
    }

    return 0;
}

void interpret(char input[], char killword[])
{
    int length = strlen(input) - 1;
    
    if (input[length] == '!')
    {
        cout << "Hey, be nice!" << endl;
    }

    else if (input[length] == '?')
    {
        cout << "I don't understand the question" << endl;
    }

    else if (strcmp(input, killword) == 0)
    {
        cout << "Whatever" << endl;
    }

    else
    {
        cout << input << endl;
    }

    cout << endl;
}
