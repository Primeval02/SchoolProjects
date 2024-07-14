#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

int read_airports(string cities[], int threshold);

int main()
{
    string cities[20];
    int count, threshold;

    cout << "Enter a threshold number: ";
    cin >> threshold;

    count = read_airports(cities, threshold);
    return 0;
}

int read_airports(string cities[], int threshold)
{
    int position, terminal_digits, counter = 3, counter2, counter3;
    char line[50], terminals[2], city[50]; 

    ifstream fin;
    fin.open("airports.txt");

    if (fin.is_open())
    {

       for(int i = 0; i < 9; i++)
        {
            getline(fin, line);

            if (fin.good())
            {
                position = strlen(line);

                if (isdigit(line[position - 1] == true);
                {
                    terminals[0] = line[position - 1];
                    terminals[1] = line[position];
                    termainal_digits = (int)(terminals);

                    if (terminal_digits >= threshold)
                    {
                        while(line[position - counter] != ' ')
                        {
                            city[counter2] = line[position - counter];
                            counter2++;
                            counter++;
                        }
                    }
                }

                else
                {
                    terminals[0] = line[position];
                    terminal_digits = (int)(terminals);

                    if (terminal_digits >= threshold)
                    {
                        while(line[position - counter] != ' ')
                        {
                            city[counter2] = line[position - counter];
                            counter2++;
                            counter++;
                        }
                    }
                }

                cities[counter3] = city;

                counter3++;
            }
        }

        fin.close();
    }

    return 0;
}
