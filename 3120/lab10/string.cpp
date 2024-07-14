#include <iostream>
#include <string.h>

using namespace std;

int main()
{
    char pattern[30];
    char text[50];
    int shift_num[256]; // Use 256 to handle ASCII characters
    cout << "Enter the pattern: ";
    cin >> pattern;
    cout << endl;
    cout << "Enter the text: ";
    cin >> text;
    int length = (int)strlen(pattern);

    // Initialize shift numbers
    for(int i = 0; i < 256; i++) {
        shift_num[i] = length; // Default shift to length of pattern
    }

    // Fill the shift table
    for (int i = 0; i < length - 1; i++) {
        shift_num[(unsigned char)pattern[i]] = length - (i + 1);
    }

    cout << endl << "Shift Table: ";
    cout << endl;
    for(int i = 'A'; i <= 'Z'; i++) {
        cout << (char)i << " ";
    }
    for(int i = 'a'; i <= 'z'; i++) {
        cout << (char)i << " ";
    }
    cout << endl;
    
    for(int i = 'A'; i <= 'Z'; i++) {
        cout << shift_num[i] << " ";
    }
    for(int i = 'a'; i <= 'z'; i++) {
        cout << shift_num[i] << " ";
    }
    cout << endl;

    // Pattern matching
    int textLength = strlen(text);
    int shifted = 0;

    while (shifted <= textLength - length) {
        int match = 0;
        for (int i = length - 1; i >= 0; i--) {
            if (pattern[i] != text[shifted + i]) {
                shifted += shift_num[(unsigned char)text[shifted + i]];
                match = -1; // Indicate mismatch
                break;
            }
        }
        if (match != -1) {
            cout << "Match found at index " << shifted << endl;
            shifted += length; // Move past this match
        }
    }

    return 0;
}


