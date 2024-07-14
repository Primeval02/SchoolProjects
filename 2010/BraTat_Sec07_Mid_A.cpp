#include <iostream>

using namespace std;

int main()
{
    string elements[500];

    for (int i = 0; i < 500; i++)
    {
        elements[i] = "BRUCE LEE";
    }

    elements[4] = "BRUCE WILLIS";

    for (int i = 0; i < 20; i++)
    {
        cout << elements[i] << ", ";
    }

    cout << endl;

    return 0;
}
