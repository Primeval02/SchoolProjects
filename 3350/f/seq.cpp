//Bradley Tate
//
//Calculate result of a sequence
//tot = + 1/1 - 1/3 + 1/5 - 1/7 + 1/9 ...
//stop when 4 decimal of precision
//after stop multiply total by 4

#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std

int main()
{
    double total, check;
    int i = 1;
    int count;

    while (true) {

        if (Round(total, 4)  == Round(check, 4))        //Will check the current total with the total of the previous iteration to check decimal precision
            break;

        check = total;

        if (count % 2 == 0)          //Adds the sequence
            total += (1 / i);

        if (count % 2 != 0)
            total -= (1 / i);

        i += 2;                     //Adds to count to determine + or - and i to increase denominator
        count++;
    }

    total *= 4.0;             //Multiplies the final total by 4 
    cout << total;

    return 0;
}
