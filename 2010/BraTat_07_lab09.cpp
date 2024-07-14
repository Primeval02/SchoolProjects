#include <iostream>
#include <cstring>

using namespace std;

bool is_palindrome(char word[]);

int main()
{
    char input[50];
    cout << "Enter a string: ";
    cin.getline(input, 50);

    if (is_palindrome(input) == true)
    {
        cout << input << " is a palindrome" << endl;
    }

    else if (is_palindrome(input) == false)
    {
        cout << input << " is not a palindrome" << endl;
    }

    return 0;
}

bool is_palindrome(char word[])
{
    int length = strlen(word);
    bool x;

    for(int i = 0; i < length; i++)
    {
        if (word[i] != word[(length - 1) - i])
        {
            x = false;
        }

        else
        {
            x = true;
        }
    }

    return x;
}
