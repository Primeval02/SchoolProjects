#include <iostream>
#include <iomanip>

using namespace std;

double calc_fare(double distance);
double calc_fare(double distance, double surcharge);
double calc_fare(double distance, bool local);
void show_fare_info(string name, string destination, double fare, bool local);

int main()
{
    string fullname;
    string destination;
    double distance;
    char local;
    double fare = 0;

    cout << fixed << setprecision(2);

    cout << "Thanks for using GUBER." << endl << endl;

    cout << "What is your full name? ";
    getline(cin, fullname);
    
    cout << "Enter destination: ";
    getline(cin, destination);

    cout << "What is the distance to " << destination << "? ";
    cin >> distance;

    cout << "Is this within the city? ";
    cin >> local;

    if (local == 'y' || local == 'Y')
    {
        fare = calc_fare(distance, true);
        show_fare_info(fullname, destination, fare, true);
    }

    else if (local == 'n' || local == 'N')
    {
        fare = calc_fare(distance, false);
        show_fare_info(fullname, destination, fare, false);
    }
    return 0;
}

double calc_fare(double distance)
{
    double fare = 0;

    if (distance <= 2)
    {
        fare = 10;
    }

    else if (distance > 2 && distance <= 5)
    {
        fare = 5 + (2.5 * distance);
    }

    else if (distance > 5)
    {
        fare = 3.5 * distance;
    }

    return fare;
}

double calc_fare(double distance, double surcharge)
{
    double fare = 0;

    fare = calc_fare(distance) + surcharge;

    return fare;
}

double calc_fare(double distance, bool local)
{
    double fare = 0;

    if (local == false)
    {
        fare = calc_fare(distance, 50.0);
    }

    else if (local == true)
    {
        fare = calc_fare(distance, 0.0);
    }

    return fare;
}

void show_fare_info(string name, string destination, double fare, bool local)
{
    if (local == true)
    {
        cout << endl << "Ok, " << name << ", your fare to " << destination << " will be $" << fare << endl;
    }

    else if (local == false)
    {
        cout << endl << "Ok, " << name << ", your fare to " << destination << " will be $" << fare << "." << endl << "This fare includes a surcharge of $50 for going outside the city limits." << endl << "Remember to use GUBER on your return trip home. We will offer a 10% " << "discount if you use GUBER on your return." << endl;
    }
}
