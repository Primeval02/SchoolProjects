#include <iostream>

using namespace std;

int main()
{
    int index = 0;
    int matches = 0;
    int count = 0;
    int x = 0;
    int pattern[] = {0,0,1,0,1,1};
    int text[] = {1,0,0,1,0,1,0,1,0,0,1,0,0,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,0,0};
    cout << "Pattern: ";
    for (int i = 0; i < 6; i++) {
        cout << pattern[i];
    }
    cout << "\nText: ";
    for (int i = 0; i < 30; i++) {
        cout << text[i];
    }
    cout << endl << endl;
    while (index < 24) {     //This # is length of text - length of pattern
        x = 0;
        for (int i = index; i <= (index + 5); i++) {
            if (text[i] != pattern[x])
                break;
            if (i == index + 5) {
                cout << "Match on position " << index << endl;
                matches++;        
            }
            x++;
            count++;
        }
        index++;
    }
    cout << "Comparisons: " << count << endl;   
    return 0;
}
