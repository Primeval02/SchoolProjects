#include <iostream>
#include <cmath>

using namespace std;

double get_double(string prompt);
double sroot(double number);
double calc_hypotenuse(double side1, double side2);
double calc_perimeter(double side1, double side2);
double calc_area(double side1, double side2);

int main()
{
    double side1, side2;
    double perimeter, area;

    side1 = get_double("Enter the length of one side ");
    side2 = get_double("Enter the length of the adjacent side ");
    perimeter = calc_perimeter(side1, side2);
    area = calc_area(side1, side2);

    cout << "A right triangle with sides " << side1 << " and " << side2 << " has a perimeter of " << perimeter << " and an area of " << area << 
         ". The length of the third side is " << calc_hypotenuse(side1, side2) << "." << endl;

    return 0;
}

double get_double(string prompt)
{
    double input;

    do
    {
        cout << prompt;
        cin >> input;
    }while(input <= 0);

    return input;
}

double sroot(double number)
{
    double root = pow(number, 0.5);
    return root;
}

double calc_hypotenuse(double side1, double side2)
{
    double hypotenuse = sroot(pow(side1, 2) + pow(side2, 2));
    return hypotenuse;
}

double calc_perimeter(double side1, double side2)
{
    double perimeter = side1 + side2 + calc_hypotenuse(side1, side2);
    return perimeter;
}

double calc_area(double side1, double side2)
{
    double area = 0.5 * side1 * side2;
    return area;
}

