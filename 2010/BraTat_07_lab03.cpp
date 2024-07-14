#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    double total;
    double gallons;
    char payment_type;
    double cost = 2.55;
    
    cout << fixed << setprecision(2);

    cout << "-- Welcome to GAS CLUB --" << endl;

    cout << "How many gallons purchased? ";
    cin >> gallons;

    cout << "Are you paying with credit or debit (c/d)? ";
    cin >> payment_type;

    if (payment_type == 'c' || payment_type == 'C')
    {
        cout << "Cost per gallon: $" << cost << endl;
        total = (gallons * cost);
        cout << "\nThank you for your purchase of " << gallons << " gallons. Please pay $" << total << "." << endl;
    }

    if (payment_type == 'd' || payment_type == 'D')
    {
        cost = 2.35;
        cout << "Cost per gallon: $" << cost << endl;
        total = (gallons * cost) + 0.35;
        cout << "\nThank you for your purchase of " << gallons << " gallons. Please pay $" << total << "." << endl;
    }
    
    cout << "All taxes are included." << endl;
    cout << "DRIVE SAFE - DON'T TEXT AND DRIVE!" << endl;

    return 0;
}
