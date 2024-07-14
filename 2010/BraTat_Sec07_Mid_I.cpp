#include <iostream>

using namespace std;

bool is_in_array(int array[], int size, int value);

int main()
{
    int array[10];
    int size, value;

    while (size < 10)
    {

        cout << "Enter an integer: ";
        cin >> value;
        
        if (is_in_array(array, size, value) == false)
        {
            array[size] = value;
            size++;
        }
    
    }

    cout << "Final array contents: ";

    for (int i = 0; i < 10; i++)
    { 
        cout << array[i] << " ";
    }

    cout << endl;

    return 0;
}

bool is_in_array(int array[], int size, int value)
{
    bool x;

    for (int i = 0; i < size; i++)
    {
        if (array[i] == value)
        {
            x = true;
            break;
            
        }

        else
        {
            x = false;
        }
    } 
        
    return x;   
}
