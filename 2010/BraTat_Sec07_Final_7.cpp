#include <iostream>
#include <iomanip>

using namespace std;

void show_time(int start_hour, int end_hour, string period);

int main()
{
    show_time(6, 8, "AM");

    return 0;
}

void show_time(int start_hour, int end_hour, string period)
{
    int minutes;

    while(start_hour < end_hour)
    {
        while(minutes < 60)
        {
            cout << start_hour << ":" << setfill('0') << setw(2) << minutes << period << endl; 
            minutes++;
        }

        minutes = 0;
        start_hour++;
    }

    cout << start_hour << ":00" << period << endl;
}

