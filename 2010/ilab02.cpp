#include <iostream>

using namespace std;

//prototypes
void say_hello();
string get_hello();
void say_something(string text);
void yell_name(string name);
int double_it(int number);
void elevator(int floor_start, int floor_end);
string get_name(string first_name, string last_name, int gender);

int main()
{
    say_hello();  //function call
    cout << get_hello() << endl;

    say_something("Something");
    say_something("debates are happening!");
    say_something("CMPS 2010");

    yell_name("Matt");
    yell_name("Joe");

    cout << double_it(10) << endl;
    cout << double_it(20) << endl;
    int x = double_it(5);
    int y;
    y = x + double_it(2);                // y is assigned 14, nothing displayed
    cout << y * double_it(5) << endl;    // 140 is displayed
    cout << double_it(y) << endl;        // 28 is displayed. y is still 14
    cout << double_it(y + 10) << endl;   // 24 is passed into double_it, so 48 is displayed

    int input;
    cout << "Enter number: ";
    cin >> input;
    cout << input << " x 2 = " << double_it(input) << endl;

    elevator(5, 12);
    elevator(7, 10);

    cout << get_name("John", "Hopkins", 1) << endl;
    cout << get_name("Kelly", "Hopkins", 2) << endl;

    return 0;
}

//definitions
void say_hello()
{
    cout << "Hello, functions" << endl;
}

string get_hello()
{
    return "Hello, functions";
}

void say_something(string text)
{
    cout << text << endl;
}

void yell_name(string name)
{
    cout << "Hey, " << name << "!" << endl;
}

int double_it(int number)
{
    number *= 2;

    return number;
}

void elevator(int floor_start, int floor_end)
{
    for (int i = floor_start; i <= floor_end; i++)
    {
        cout << "Floor " << i << endl;
    }
}

string get_name(string first_name, string last_name, int gender)
{
    string full = last_name + ", " + first_name;

    if (gender == 1)
    {
        full = "Mr. " + full;
    }

    else if (gender == 2)
    {
        full = "Ms. " + full;
    }

    return full;
}
