#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    double daily_pay = 0.01; 
    double total_pay;
    cout << fixed << setprecision(2);

    
    for (int i = 1; i <= 30; i++)
    {
        cout << "On day " << i << ", you made $" << daily_pay << endl;
        total_pay = total_pay + daily_pay;
        daily_pay = daily_pay * 2;

    }

    cout << "After 30 days, you made $" << total_pay << endl;
    return 0;
}
