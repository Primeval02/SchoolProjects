// Author:  Melissa Danforth

// Purpose: Test out the quicksort routines

//          This code assumes you have the file quicksort.cpp and vt100ansi.h

//          provided in the lab in the same directory as this file when you

//          are compiling the program.


#include <iostream>
#include <fstream>
#include "quicksort.h"
using namespace std;

#define MAX_CAPACITY   1000

int main(int argc, char *argv[])
  {
  //  1. create an int array of size MAX_CAPACITY

  int a[MAX_CAPACITY];

  if(argc < 2)
    {
    cout << "Usage: " << argv[0] << " <filename>\n";
    return 1;
    }

  ifstream fin;
  int count = 0;
  //  2. Open the file specified on the command line

  fin.open(argv[1]);

  //  3. Verify that the file open succeeded

  if(fin.fail())
    {
    cout << "Error: Could not open " << argv[1] << " for reading.\n";
    return 1;
    }

  //  4. Read data in from the file until either the end of file is reached
//     or you have read MAX_CAPACITY elements (use 'count' to track the
//     number of elements read in to the array)
   while (!fin.eof())
   { 
       if (count == MAX_CAPACITY)
       {
           break;
       }
       fin >> a[count];
       count++;
   }

   count--;
   fin.close();
// 5. Call the quicksort algorithm to sort the data
    quicksort(a, 0, count - 1);
  return 0;
  }
