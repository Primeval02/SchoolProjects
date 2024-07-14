/*   static arrays versus dynamic arrays      */

#include "cmpslib19.h"
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP



void PrintArray(int*array,int size)
  {
      LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
  cout << "Array Address" << array << endl;
  for(int loop =0; loop<size; loop++)
    {
    if(loop)
      {
      cout << ",";
      }
    cout << array[loop];
    }
  cout << endl;
      LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
  }

int main()
  {
	InitializeLogger("main1.log");



// create a static int array of size 20 named SIntArray
int SIntArray[20];

// create a dynamic int array of size 20 named DIntArray
int * DIntArray;
DIntArray = new int [20];

// use a for loop to populate both arrays with the values 0-19 , use the [] operator
for (int i = 0; i < 20; i++)
{
    SIntArray[i] = i;
    DIntArray[i] = i;
}
// use the PrintArray functions to print them both out
cout << "SIntArray: " << endl;
PrintArray(SIntArray, 20);
cout << "DIntArray: " << endl;
PrintArray(DIntArray, 20);

// use a for loop to populate both arrays with the values 100-119 (loop+100) ,   do not use the [] operator
for (int i = 0; i < 20; i++)
{
    *(SIntArray + i) = (i + 100); 
    *(DIntArray + i) = (i + 100);
}

// use the PrintArray functions to print both arrays
cout << "SIntArray: " << endl;
PrintArray(SIntArray, 20);
cout << "DIntArray: " << endl;
PrintArray(DIntArray, 20);

// print out the address of the first  , second and fifth element of both arrays, use the & and [] operators
cout << "SIntArray: " << endl;
cout << "1st: " << &SIntArray[0] << endl;
cout << "2nd: " << &SIntArray[1] << endl;
cout << "5th: " << &SIntArray[4] << endl;
cout << "DIntArray: " << endl;
cout << "1st: " << &DIntArray[0] << endl;
cout << "2nd: " << &DIntArray[1] << endl;
cout << "5th: " << &DIntArray[4] << endl;
// print out the address of the first  , second and fifth element of both arrays, do NOT  use the & and [] operators
cout << "SIntArray: " << endl;
cout << "1st: " << SIntArray + 0 << endl;
cout << "2nd: " << SIntArray + 1 << endl;
cout << "5th: " << SIntArray + 4 << endl;
cout << "DIntArray: " << endl;
cout << "1st: " << DIntArray + 0 << endl;
cout << "2nd: " << DIntArray + 1 << endl;
cout << "5th: " << DIntArray + 4 << endl;

// create an int * temp
int * temp;

// set temp to the begining of SIntArray
temp = SIntArray;

// increment temp pointer by 4
temp += 4;

//  print out the value ( address ) of temp  and the value it points to
cout << "temp contains the address " << temp << " and points to the value " << *temp << endl;

// set temp to the begining of DIntArray
temp = DIntArray;

// increment the pointer by 4
temp += 4;

//  print out the value ( address ) of temp  and the value it points to
cout << "temp contains the address " << temp << " and points to the value " << *temp << endl;

// clean up the Dynamic array
delete[] DIntArray;

  return 0;
  }






