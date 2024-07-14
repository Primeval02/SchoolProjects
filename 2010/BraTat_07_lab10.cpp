#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

void parse_name(char full[], char first[], char last[]);
void create_timecard(double hours[], char first[], char last[]);

int main()
{
    char full[40], first[20], last[20];
    double hours[5];
    double hours_per_day;

    cout << "What is your name? ";
    cin.getline(full, 40);

    for(int i = 1; i <= 5; i++)
    {
        cout << "Enter hours for day " << i << ": ";
        cin >> hours_per_day;
        hours[i - 1] = hours_per_day;
    }

    parse_name(full, first, last);
    create_timecard(hours, first, last);
    
    cout << "Timecard is ready. See timecard.txt" << endl;
    return 0;
}

void parse_name(char full[], char first[], char last[])
{
    int count = 0;
    int count2 = 0;
    int position = strlen(full);
  
    while (full[count] != ' ')
    {
        first[count] = full[count];
        count++;
    }

    first[count] = '\0';
    count++;

    while (full[count] != '\0')
    {
        last[count2] = full[count];
        count++;
        count2++;
    }

    last[count2] = '\0';
} 

void create_timecard(double hours[], char first[], char last[])
{
    double total = 0;

    ofstream fout;
    fout.open("timecard.txt");

    if(fout.is_open())
    {
        fout << "First Name: " << first << endl;
        fout << "Last Name: " << last << endl;
        
        for(int i = 0; i < 5; i++)
        {
            fout << "Day " << i + 1 << ": " << hours[i] << endl;
            total += hours[i];
        }

        fout << "Total Hours: " << total << endl;
        fout.close();
    }
}
