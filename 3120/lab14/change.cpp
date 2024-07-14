#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    int n;
    cout << "Enter amount to break down: ";
    cin >> n;

    vector<int> values{0, 1, 2, 3, 4, 1, 2, 3, 4, 5, 2, 1};

    int small;

    for (int i = 12; i < n + 1; i++) {
        small = min(values[i - 11], values[i - 5]);
        small = min(values[i - 1], small);
        small += 1;
        values.push_back(small);
    }

    cout << "It would only take " << values[n] << " coins." << endl;
}
