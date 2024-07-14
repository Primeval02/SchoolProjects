#include <iostream>

using namespace std;

int main()
{
    int days = 365;
    int minutes = 60;
    int hours = 24;
    string message = "Number of minutes: ";
    double actual_days = 365.2422;
    int total = minutes * hours * days;
    
     
    cout << message << total << endl;
    cout << "Number of minutes in a Tropical Year: " << minutes * hours * actual_days << endl;

    return 0;
}
