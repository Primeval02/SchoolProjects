// Bradley Tate
// Final Problem #8
// 12/14/2020

#include <iostream>
#include <fstream>

using namespace std;

class Account 
{
    // use the correct access specifier below such that
    // the variables, number and balance, are available
    // to child classes of Account
    public:
        string number;
        double balance;
};

// implement Checking class as a child of Account below
class Checking : public Account
{
    private:
        string routing;

        double get_trans_total()
        {
            int counter;
            string temp_number;

            ifstream fin;

            fin.open("account.dat");

            if(fin.is_open())
            {
                string line;
                while(!fin.eof())
                {
                    getline(fin, line);
                    if (fin.good())
                    {
                        for (int i = 1; i < line.length(); i++)    //For loop searches for capital char C or D after first char in line.  
                        {                                          //Had block of code that received numbers after, could not convert into double and too many bugs to compile
                            if (line[i] == 'C')                    //Deleted it to start again, but ran out of time.
                            {
                                balance += 
                            }

                            else if (line[i] == 'D')
                            {
                                balance -=                                 
                            }
                        }
                    }
                }

                fin.close();
            }

            return balance;
        }

    public:
        Checking(string account, string route)
        {
            number = account;
            routing = route;
            get_trans_total();
        }
        
        void create_statement()
        {
            ofstream fout;
            fout.open("statement.txt");
            if (fout.is_open())
            {
                fout << "Account: " << number << endl;
                fout << "Routing: " << routing << endl;
                fout << "Balance: " << balance << endl;
                fout.close();
            }
        }
};


int main()
{
    // ##### YOU SHOULDN'T NEED TO DO ANYTHING ELSE IN MAIN #####

    // instantiate acct object 
    // with routing and checking numbers
    Checking acct("01234567", "333444000");

    // create text file of account statement
    acct.create_statement();

    return 0;
}


// Congratulations on getting to the end of the semester
// For an additional bonus + 0.5pts
// What C++ topic were we discussing Sarah Connor? 
//
// Char Arrays, but more specifically, null terminators
