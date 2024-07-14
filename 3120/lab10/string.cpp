#include <iostream>
#include <string.h>

using namespace std;

int main()
{
    char pattern[30];
    char text [50];
    char letters [26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    int shift_num [26];
    cout << "Enter the pattern: ";
    cin >> pattern;
    cout << endl;
    cout << "Enter the text: ";
    cin >> text; 
    int length = (int)strlen(pattern);

    for(int i = 0; i < 26; i++) {
        shift_num[i] = length; 
    }

    for (int i = 0; i < (length - 1); i++) {
        shift_num[pattern[i] - 65] = length - (i + 1);
    }

    cout << endl << "Shift Table: ";
    cout << endl;
    for(int i = 0; i < 26; i++) {
        cout << letters[i] << " ";
    }
    cout << endl;
    for(int i = 0; i < 26; i++) {
        cout << shift_num[i] << " ";
    }
    cout << endl; 

    //END OF SHIFT TABLE
/*    
    bool done = true;
    int shifted, match = 0;
    char checkchar = 'A';
    while (done) {                                              //If the checked text char is not = to the last pattern's: shift
        if (pattern[length - 1] != text[length + shifted]) {
            checkchar = text[length + shifted];
            shifted += shift_num[checkchar - 65];
        }
        if ((length + shifted) > (int)strlen(text))             //If we exceed text length: end
            done = true;
        else {
            for (int i = 0; i < length; i++) {                  //If every character from text matches add 1 to match and continue 
                if (pattern[i] != text[length + shifted - i])
                    break;
                else {
                    match++;
                    shifted += length;
                }
            }
        }
    }*/
}

