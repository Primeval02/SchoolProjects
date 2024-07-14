#include <iostream>

using namespace std;

int output(int a, double b)
{
    if (a < 650)
    {
        cout << "\nUnfortunately, your application has been denied." << endl;
        cout << "You may apply again in 6 months." << endl << endl;
    }

    else if (a > 650)
    {
        if (a >= 650 && a <= 720)
        {
            b += 3.5;
        }

        if (a >= 721 && a <= 850)
        {
            b += 1.9;
        }

        cout << "\nYour application has been APPROVED! Your rate is " << b << "%" << endl << endl;
    }

    return b;
}




int main()
{
    int score;
    char discharged;
    char bankruptcy;
    double rate = 0.0;

    cout << "\n-- Thank you for applying for our loan --" << endl;
    cout << "What is your credit score? ";
    cin >> score;
    cout << "Have you had a bankruptcy? ";
    cin >> bankruptcy;

    if (bankruptcy == 'y' || bankruptcy == 'Y')
    {
        cout << "Has your bankruptcy been discharged? ";
        cin >> discharged;

        if (discharged == 'n' || discharged == 'N')
        {
            cout << "\nUnfortunately, your application has been denied." << endl;
            cout << "You may apply again in 6 months." << endl << endl;
        }

        else if (discharged == 'y' || discharged == 'Y')
        {
            rate += 3.0;
            output (score, rate);

        }
            
    }
    
    else if (bankruptcy == 'n' || bankruptcy == 'N')
    {
        output (score, rate);
    }
   
    return 0;
}
