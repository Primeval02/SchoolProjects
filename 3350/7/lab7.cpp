//
//Starting framework for lab-7
//
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
using namespace std;
const int MAXC = 200;

int main(int argc, char *argv[])
{
	int sum[MAXC] = {0};
	srand((unsigned)time(NULL));
	int rows = 2;
	int columns = 4;
    if (argc > 1){
        rows = atoi(argv[1]);
    }
    if (argc > 2){
        columns = atoi(argv[2]);
    }

    cout << "Lab-7 Sum of large numbers" << endl;
    cout << "--------------------------\n" << endl;
    cout << "Enter rows & columns: " << rows << " " << columns << endl << endl;

	for (int i=0; i<rows; i++) {
		cout << "    ";
		for (int j=0; j<columns; j++) {
			int r = rand() % 10;
			cout << r;
			sum[j] += r;
		}
        cout << endl;
	}
	cout << endl;
    //Displays answer
    //First: do carrying from right to left
    for (int j = columns - 1; j > 0; j--){
        int right_digit = sum[j] % 10;
        int carry = sum[j] / 10;
        sum[j] = right_digit;
        sum[j-1] += carry;
    }
    int pad = log10(sum[0]);
    cout << setw(4 - pad) << " ";
	for (int j=0; j<columns; j++) {
		cout << sum[j];
	}
	cout << endl;
    //make a final sum array with enough didits for carrying sum[0]
	cout << "\nprogram complete." << endl;
	return 0;
}




