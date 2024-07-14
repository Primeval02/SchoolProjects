#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    double previous_price;
    double current_price;
    string symbol;
    string company;
    
    cout << fixed << setprecision(2);

    cout << "-- Welcome to CSUB Brokerage --" << endl;

    cout << "What is the stock symbol? ";
    cin >> symbol;

    cout << "What is the company name? ";
    cin >> company;

    cout << "What was the previous stock price? ";
    cin >> previous_price;

    cout << "What is the current price? ";
    cin >> current_price;

    if (previous_price > current_price)
    {
        double decrease_percentage = ((previous_price - current_price) / previous_price) * 100;
        cout << endl << "It's a bear market! " << company << " (" << symbol << ") " << "went down " << 
            decrease_percentage << "%." << endl;
    }
    
    if (previous_price < current_price)
    {
        double increase_percentage = ((current_price - previous_price) / previous_price) * 100;
        cout << endl << "I's a bull market! " << company << " (" << symbol << ") " << "went up " << 
           increase_percentage << "%." << endl; 
    }

    if (previous_price == current_price)
    {
        cout << endl << "I's a flat market! " << company << " (" << symbol << ") " << "stock price didn't change." 
            << endl;
    }

    return 0;
}
