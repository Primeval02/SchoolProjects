#include <iostream>

using namespace std;

int main()
{
    // Problem 1

    string user_string;
    int times_repeated = 0;
    int count = 0;

    cout << "Enter a string to be repeated: ";
    cin >> user_string;
    cout << "How many times to show? ";
    cin >> times_repeated;
    cout << endl;

    while (count < times_repeated)
    {
        cout << user_string << endl;
        count++;
    }
    
    cout << endl;

    // Problem 2

    int integers = 1;
    
    while (integers <= 20)
    {
        cout << integers << " ";
        integers += 1;
    }
  
    cout << endl;

    // Problem 3

    int user_integer;
    
    cout << "\nEnter a number larger than 100: ";
    cin >> user_integer;

    while (user_integer > 0)
    {
        if (user_integer % 10 == 0)
        {
            cout << user_integer << "\t";
        }
        
        user_integer--;
    }

    cout << endl;

    // Problem 4

    int user_integer2;
    bool positive = true;

    while (positive)
    {
        cout << "\nEnter a number: ";
        cin >> user_integer2;

        if (user_integer2 < 0)
        {
            positive = false;
        }

        else if (user_integer2 >= 0)
        {
            cout << user_integer2 << " x 2 = " << (user_integer2 * 2);
        }
        
    }

    cout << endl;

    // Problem 5

    string user_string2;
    string keyword = "bye";

    while (user_string2 != keyword)
    {
        cout << "Enter a string: ";
        cin >> user_string2;
    }


    return 0;
}
