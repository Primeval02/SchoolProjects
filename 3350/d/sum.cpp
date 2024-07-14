//By: Bradley Tate 
//4/28/2022

#include <iostream>

using namespace std;

int main()
{
    cout << "Summation" << endl;
    cout << "---------" << endl;
    cout << "\nSummation to what number? ";
    int limit, sum, count, count2;           //Declare variables
    cin >> limit;
    cout << "\ncalculating now..." << endl;

    //------------------------------------------------Summation 
    for (int i = 0; i < 10; i++) {
        count = 0;                         //reset while count
        while (count < 10) {
            count2 = 0;                    //reset do while count
            do {
                sum = 0;
                for (int i = 1; i <= limit; i++) {
                    sum += i;
                }
                count2++;
            } while (count2 < 10);
            count++;
        }
    }

    cout << "calculation complete." << endl;
    cout << "\nThe summation is: " << sum << endl;
    return 0;
}


