#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    char sentence[127];
    int count;

    cout << "Enter a sentence that does not exceed 127 characters: ";
    cin.getline(sentence, 127);

    for (int i = 0; i < 127; i++)
    {
        if (sentence[i] == 'e')
        {
            count++;
        }
    }

    cout << "The letter 'e' appeared " << count << " time(s)." << endl;
    return 0;
}
