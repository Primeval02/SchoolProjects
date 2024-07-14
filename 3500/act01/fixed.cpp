/***********************************************/
/* NAME: Bradley Tate (originally by Walter) */
/* ASGT: Activity 1 */
/* ORGN: CSUB - CMPS 3500 */
/* FILE: fixed.cpp */
/* DATE: 02/11/2023 */
/***********************************************/

#include <iostream>

using namespace std;

int findFib(int arr[], int start, int length, int input) /* start is starting index */
{
    if (length >= start){
        int mid = start + (length - start) / 2; /* midpoint */
        if (arr[mid] == input)
            return mid; 
        if (arr[mid] > input)
            return findFib(arr, start, mid - 1, input); /* will change the max length and search left of midpoint */
        return findFib(arr, mid + 1, length, input); /* will change the starting index and search right of midpoint */ 
    }
    return -1;
}

int main(void)
{
    int arr[] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181 }; 
    int i = 0; 
    int length = sizeof(arr) / sizeof(arr[0]);
    while (i != 1){ 
        int input; 
        int result; 
        cout << "Type a fibonacci number less than 100: ";
        cin >> input; 
        result = findFib(arr, 0, length - 1, input); 
        if (result == -1){
            cout << "The number you entered is not a fibonacci number, please try again!\n"; 
        } else{
                cout << "The number you typed is the " << result << "-th fibonacci number, good bye!\n"; 
                i = i + 1; 
        }
    } 
    return 0; 
}
