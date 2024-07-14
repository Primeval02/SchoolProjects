#include <iostream>
#include <cstring>

using namespace std;

void smallify(char before[], char after[]);

int main()
{
    char input[50];
    char output[50];
    cout << "Enter a string: ";
    cin.getline(input, 50);

    smallify(input, output);

    cout << input << " becomes " << output << endl; 

    return 0;
}

void smallify(char before[], char after[])
{
    int position = strlen(before) - 1;
    
    for (int i = 0; i < position + 1; i++)
    {
        after[i] = before[i];
    }

    if (before[position] == 'a' && before[position - 1] == 'c')
    {
        after[position - 1] = 'q';
        after[position] = 'u';
        after[position + 1] = 'i';
        after[position + 2] = 't';
        after[position + 3] = 'a';
        after[position + 4] = '\0';
    } 

    else if (before[position] == 'o' && before[position - 1] == 'c')
    {
        after[position - 1] = 'q';
        after[position] = 'u';
        after[position + 1] = 'i';
        after[position + 2] = 't';
        after[position + 3] = 'o';
        after[position + 4] = '\0';
    }
}
