#include <iostream>

using namespace std;

int main() {
    int input1, input2;
    int increment = 1;
    cout << "Enter 2 positive integers: ";
    cin >> input1;
    cin >> input2;

    while (true) {
        if (increment % input1 == 0 && increment % input2 == 0) {
            cout << "The LCM of " << input1 << " and " << input2 << " is " << increment << endl;
            return increment;
        }
        increment++;
    }
}
