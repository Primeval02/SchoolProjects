#include <iostream>
#include <fstream>

using namespace std;

void copy_file(string file_name);

int main()
{
    string file_name;

    cout << "Enter the name of an existing text file: ";
    cin >> file_name;

    copy_file(file_name);

    cout << "The copy of " << file_name << " is ready and called: copy_" << file_name << endl;
    return 0;
}

void copy_file(string file_name)
{
    ifstream fin;
    fin.open(file_name);

    if (fin.is_open())
    {
        ofstream fout;
        fout.open("copy_" + file_name);

        if (fout.is_open())
        {
            string line;

            while(!fin.eof())
            {
                getline(fin, line);

                if(fin.good())
                {
                    fout << line << endl;
                }

            }

            fout.close();
        }

        fin.close();
    }
}
