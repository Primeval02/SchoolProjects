#include <iostream>

using namespace std;

int main()
{
    int input, quarters, dimes, nickels, pennies;
    cout << "Enter amount of money in cents: ";
    cin >> input;

    quarters = input / 25;
    input = input % 25;
    dimes = input / 10;
    input = input % 10;
    nickels = input / 5;
    input = input % 5;
    pennies = input;

    cout << "Change given using least amount of coins: " << quarters << " quarters, " << dimes << " dimes, " << nickels << " nickels, and " << pennies << " pennies." << endl;
}
