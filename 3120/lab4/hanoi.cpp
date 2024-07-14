#include <iostream>

using namespace std;

int count;

void hanoi(int disk, int start, int end, int extra)
{
    if (disk == 0)
        return;

    else {
        hanoi(disk - 1, start, extra, end);
        cout << "Move disk " << disk << " from pole " << start << " to pole " << end << endl;
        hanoi(disk - 1, extra, end, start);    
        count++;   
    }
}

int main()
{
    int disks;
    cout << "How many disks? ";
    cin >> disks;

    hanoi(disks, 1, 3, 2);
    cout << "\nNumber of moves: " << count << endl;
    return 0;
}
