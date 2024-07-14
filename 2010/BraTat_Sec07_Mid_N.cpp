#include <iostream>
#include <fstream>

using namespace std;

bool include_file(string header_file_name);

int main()
{ 
    string headers[5] = {"cstdlib", "cstring", "fstream", "cmath", "iomanip"};
    string file_name;

    cout << "What is the name of your target source file? ";
    cin >> file_name;

    ofstream fout;

    fout.open(file_name);
    
    if (fout.is_open())
    {
        fout << "#include <iostream>" << endl;

        for (int i = 0; i < 5; i++)
        {
            if (include_file(headers[i]) == true)
            {
                fout << "#include <" << headers[i] << ">" << endl;
            }
        }

        fout << endl << "using namespace std;" << endl << endl;
        fout << "int main()" << endl;
        fout << "{" << endl << endl;
        fout << "    return 0;" << endl;
        fout << "}" << endl;

        fout.close();
    }

    cout << "Your file " << file_name << " is ready." << endl;
    return 0;
}

bool include_file(string header_file_name)
{
    char answer;
    bool x;

    cout << "Include " << header_file_name << "? ";
    cin >> answer;

    if (answer == 'n' || answer == 'N')
    {
        x = false;
    }

    else if (answer == 'y' || answer == 'Y')
    {
        x = true;
    }

    return x;
}
