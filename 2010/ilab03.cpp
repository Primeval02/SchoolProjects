#include <iostream>

using namespace std;

class Date
{
    private:
        int month, day, year;
    public:
        Date(int m, int d, int y)
        {
            month = m;
            day = d;
            year = y;
        }

        string get_date_str()
        {
            return to_string(month) + "/" + to_string(day) + "/" + to_string(year);
        }

        Date operator+(int addlday)
        {
            /*
            // Option 1
            day = day + addlday;
            // "this" pointer, it points to itself (current instance)
            return *this
            */

            // Option 2 
            Date temp(month, day, year);
            temp.day = temp.day + addlday;
            return temp;
        }

        Date operator^(int addlmonth)
        {

            month = month + addlmonth;

            if (month > 12)
            {
                month = month - 12;
                year++;
            }

            return *this;
        }

        ~Date()
        {
            cout << "Destructing..." << endl;
        }
};

int main()
{
    Date d1(11, 20, 2000);

    cout << d1.get_date_str() << endl;

    d1 = d1 + 10;        // + means add a day

    cout << d1.get_date_str() << endl;

    d1 = d1 ^ 4;         // ^ means add addl month(s)

    cout << d1.get_date_str() << endl;

    cout << "Last line in main" << endl;
    return 0;
}
