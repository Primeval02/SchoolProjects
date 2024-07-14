#include <iostream>

using namespace std;

int main() {
    double storage[1000];
    storage[0] = 0;
    storage[1] = 1;
    int n, count;
    cout << "What is n? ";
    cin >> n;
    for (int i = 2; i <= n; i++) {
        storage[i] = storage[i - 1] + storage[i - 2];
        count++;
    }
    cout << "Final number: " << storage[n] << endl;
    cout << "Steps used: " << count << endl;
    return 0;
}
