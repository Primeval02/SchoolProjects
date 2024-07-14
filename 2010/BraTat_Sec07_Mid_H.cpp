#include <iostream>
#include <fstream>

using namespace std;

void post(char text[], int max);

int main()
{
    char text[100];
    cout << "Enter a string with no more than 100 characters: ";
    cin.getline(text, 100);

    post(text, 25);

    return 0;
}

void post(char text[], int max)
{
    ofstream fout;

    fout.open("post.txt");

    if (fout.is_open())
    {
        for (int i = 0; i < max; i++)
        {
            fout << text[i];
        }

        fout << endl;

        fout.close();
    }
}
