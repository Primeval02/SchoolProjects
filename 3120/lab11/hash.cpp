#include <iostream>

using namespace std;

void linear(int* input)
{
    int table[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 7; i++) {
        int num = input[i] % 21;

        if (table[num] == 0) {
            table[num] = input[i];
        }

        else {
            while(table[num] != 0)
                num++;
            table[num] = input[i];
        }
    }
    for(int i = 0; i < 20; i++) {
        cout << table[i] << " "; 
    }
    cout << endl;
}

void chained(int* input)
{
    int table[10][10];
    int col;
    for (int i = 0; i < 7; i++) {
        col = 0;
        int num = input[i] % 21;
        cout << "input: " << input[i] << " mod: " << num << endl; 
        if (table[num][col] == 0)
            table[num][col] = input[i];
        else {
            while (table[num][col] != 0)
                col++;
            table[num][col] = input[i];
        }
    } 

    for(int a = 0; a < 10; a++)
    {
        for(int b = 0; b < 10; b++)
        {
            cout << table[a][b] << " ";
        }
        cout << endl;
    } 
}

int main()
{
    int input[7] = {7, 22, 44, 27, 38, 53, 40};
    linear(input);
    return 0;
}
