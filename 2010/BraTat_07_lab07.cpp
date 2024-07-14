#include <iostream>

using namespace std;

double get_money(string prompt);
double get_money_min(string prompt, double minimum);
void calc_change(double change, int & quarters, int & dimes, int & nickels, int & pennies);

int main()
{
    int quarters = 0, dimes = 0, nickels = 0, pennies = 0;

    double total = get_money("Enter a total: ");
    double tendered = get_money_min("Enter amount tendered: ", total);
    double change = tendered - total;
    cout << endl << "Your change is " << tendered - total << "." << endl;
    calc_change(change, quarters, dimes, nickels, pennies);
    cout << "You should have " << quarters << " quarter(s), " << dimes << " dime(s), " << nickels << " nickel(s), and " << pennies << " penny/ies." << endl;
    return 0;
}

double get_money(string prompt)
{
    double total;

    do
    {
        cout << prompt;
        cin >> total;
    }while(total < 0);

    return total;

}

double get_money_min(string prompt, double minimum)
{
    double input;

    do
    {
        cout << prompt;
        cin >> input;
        if (input < minimum)
        {
            cout << "*** Amount must be at least " << minimum << endl;
        }
    }while(input < minimum);

    return input;
}

void calc_change(double change, int & quarters, int & dimes, int & nickels, int & pennies)
{
    int cents = change * 100;
    cents = cents % 100;
    quarters = cents / 25;
    cents = cents % 25;
    dimes = cents / 10;
    cents = cents % 10;
    nickels = cents / 5;
    cents = cents % 5;
    pennies = cents;
}
