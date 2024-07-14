#include <iostream>

using namespace std;

int main()
{
    int arr[] = {8, 3, 1, 2, 0, 2, 5, 4, 7, 3};
    int arrsize = 10;
    int check, count;

    cout << "Input: ";
    for (int i = 0; i < arrsize; i++) {
        cout << arr[i] << ", ";
    }
    cout << endl;

    for (int i = 1; i < arrsize; i++) {              //Sort
        check = arr[i];                              //Will reset to the value of the highest element every iteration
        int j = i - 1;                               //Previous term

        while (j >= 0 && arr[j] > check) {           //Checks if element to the left is not term 0 and its value is larger than current element
            arr[j + 1] = arr[j]; 
            j = j - 1;
            count++; 
        }
        arr[j + 1] = check;                          //reset arr[i] 

        for (int i = 0; i < arrsize; i++) {          //prints every sort step
            cout << arr[i] << ", ";
        }
        cout << endl;
    }

    cout << "Final: ";
    for (int i = 0; i < arrsize; i++) {              //Final array
        cout << arr[i] << ", ";
    }
    cout << endl;
    cout << "Basic Operation was executed " << count << " times." << endl;

    return 0;
}
