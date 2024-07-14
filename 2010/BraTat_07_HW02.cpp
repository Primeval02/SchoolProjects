#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    double balance;
    double monthly;
    double apr;
    double apr_monthly;
    double start_bal;
    double interest;
    double end_bal;
    int months = 1;
    
    cout << fixed << setprecision(2);

    cout << "What is the outstanding balance? ";
    cin >> balance;
    cout << "What is the APR? ";
    cin >> apr;

    do
    {

     cout << "How much do you pay every month? ";
     cin >> monthly;

    }while (monthly <= 0);
    
    start_bal = balance;
    cout << endl << "Month     Start   Interest      Monthly     End" << endl;
    do
    {
        apr_monthly = (apr / 12) / 100;
        interest = start_bal * apr_monthly;
        end_bal = start_bal + interest - monthly;

        cout << setw(5) << months << setw(10) << start_bal << setw(8) << interest << setw(15) << monthly << setw(9) << end_bal << endl;

        start_bal = end_bal;
        months ++;
           
    }while (end_bal > 0);

    return 0;
}
