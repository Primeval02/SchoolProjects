//Bradley Tate
//lab1-3600

#include <iostream>
#include <fstream>

using namespace std;

int main(void)
{
    char name[100];
    cout << "What is your name: ";
    cin >> name;
    cout << name << endl;
    ofstream fout;
    fout.open("myfile.txt");
    fout << name << endl;
    fout.close();

    return 0;
}
