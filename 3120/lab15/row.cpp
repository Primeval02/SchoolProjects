#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

using namespace std;

int main()
{
    int coins[] = {0, 15, 12, 3, 7, 6, 5, 9, 19, 2, 21, 12, 5, 9, 6, 14, 20, 1, 3, 7, 12, 14, 7, 3, 21, 15};
    vector<int> total = {0, 5};

    int n = sizeof(coins) / sizeof(coins[0]);
    int maximum;

    for (int i = 2; i < n; i++) {
        maximum = max((coins[i] + total[i - 2]), total[i - 1]);
        total.push_back(maximum);
    } 

    cout << "The maximum you can grab is " << total[n - 1] << endl;
}
