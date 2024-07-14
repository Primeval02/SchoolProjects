#include <iostream>

using namespace std;

string get_date(int day);

int main()
{
    int day;

    cout << "Enter the nth day of the year: ";
    cin >> day;
    cout << "Day " << day << " is " << get_date(day) << endl;

    return 0;
}

string get_date(int day)
{
    string months[12] = {"Jan", "Feb", "Mar", "April", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int day_of_month, month;
   
    while(day > days[month])
    {
        day -= days[month];
        month += 1;
    }

    day_of_month = day;

    return months[month] + " " + to_string(day_of_month);
}
