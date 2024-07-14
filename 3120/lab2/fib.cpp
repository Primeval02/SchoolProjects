#include <iostream>

using namespace std;

int fib (int end); 

double count;

int main()
{
    double n;
    cout << "What is n? ";
    cin >> n;
    n = fib(n);
    cout << "The final number: " << n << endl;
    cout << "Steps used: " << count << endl;
    return 0;
}

int fib(int n)
{
    if (n <= 1)
        return n;
    else {
        count++;
        return (fib(n - 1) + fib(n - 2));
    }
}
