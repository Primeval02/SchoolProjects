//Author: Bradley Tate
//Written on: 3/3/2022 and Revised on 3/9/2022

#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

const char fname[] = "/home/fac/gordon/public_html/3350/dictionary";

bool check_dupe(char *arr);
//void sort_array(char *arr);

int main(int argc, char *argv[]){
    int size = 1;
    if (argc > 1){
       size = atoi(argv[1]); 
    }

    else{
        cout << "Please enter a size larger than 1 by using ./lab6 'x'" << endl;
        exit(0);
    }

    cout << "3350 Lab-6 Challenge #1" << endl;
    cout << "The " << size << " longest words with no duplicate letters." << endl << endl; 

    ifstream fin;
    fin.open(fname);
    char word[100];
    int n = 0;
    //char **tosort = new char *[size];                       //Array of all the compiled words of no duplicates
    while(n < size){
        fin >> word;
        if (check_dupe(word) == 0){             //If the check_dupe did not find a duplicate it will print the word for now but should add it to the final array
            printf("%s ", word);
            //tosort[n] = word;                 //If the word is not a duplicate it is added to the final array
            n++;
        }
    }
    cout << "\n";
    fin.close();
    //sort_array(tosort);                      //Calls the final array to be sorted by size and printed

    cout << "\nEnd of program." << endl;
    return 0;
}

bool check_dupe(char *arr){                    //Checks for duplicates in a word and returns it in "check" with 1 being a duplicate
    bool check = 0;
    for (size_t i = 0; i < strlen(arr); i++){                  
        for (size_t j = i + 1; j < strlen(arr); j++){
            if (arr[i] == arr[j]){                  
                check = 1;                 
            }
        }
    }
    return check;
}

/*
void sort_array(char *arr){                   //Will sort the final array of words by size using a bubble sort
    for (int i = 0; i < arr.length; i++){
        for (int j = i + 1; j < arr.length; j++){
            if (arr[j] > arr[i]){
                char temp[100] = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    for (int i = 0; i < arr.length; i++){
        cout << arr[i] << " ";
    }
}
*/
