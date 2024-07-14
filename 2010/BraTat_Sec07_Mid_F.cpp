#include <iostream>
#include <cstring>

using namespace std;

void as_title_case(char source[], char dest[]);

int main()
{
    char source[127];
    char dest[127];

    cout << "Enter a string, all lower case that does not exceed 127 characters: ";
    cin.getline(source, 127);
    as_title_case(source, dest);

    cout << "In Title Case: " << dest << endl;
    return 0;
}

void as_title_case(char source[], char dest[])
{
    for (int i = 0; i < strlen(source); i++)
    {
        dest[i] = source[i];
    }

    dest[0] = toupper(dest[0]);

    for (int i = 0; i < strlen(dest); i++)
    {
        if (dest[i] == ' ')
        {
            dest[i + 1] = toupper(dest[i + 1]);
        }
    }
}
