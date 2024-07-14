
#include "cmpslib19.h" // all the special functions provided for this class

// INCLUDE THE LIBRARY FOR THE LOGGING FUNCTIONS AND THE MACRO TO INITIALIZE IT
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP


// put your functions here
int TripleInt(int val)
  {
  LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
  int result =  val *=3;
  LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
  return result;
  }

int SmallerInt(int value1, int value2)
{
    LOG(INFO) << "Start: SmallerInt " << __PRETTY_FUNCTION__ << endl;
    int smaller = (value1 <= value2) ? value1: value2;
    return smaller;
    LOG(INFO) << "End: SmallerInt " << __PRETTY_FUNCTION__ << endl;
}

int LargerInt(int value1, int value2)
{
    LOG(INFO) << "Start: LargerInt " << __PRETTY_FUNCTION__ << endl;
    int larger = (value1 >= value2) ? value1: value2;
    return larger;
    LOG(INFO) << "End: LargerInt " << __PRETTY_FUNCTION__ << endl;
}

int main()
  {
  InitializeLogger("main1.log");



  cout << endl << endl;
  cout <<"Testing TripleInt" << endl;

  int temp;
  cout << "TripleInt(1): ";
  temp = TripleInt(1);
  if ( temp == 3 )   // test the value returned by the function to ensure it is correct  
    cout << "Pass" << endl;
  else
    cout << "Fail" << endl;

   /* 
	PassFail( bool)  is a function that returns either "Pass" or "Fail"
	depending on the expressions value
  put an exprssion that evalueates to TRUE only if your your function actually came back with the correct answer
  */

  cout << "TripleInt(3): ";
  temp = TripleInt(3);
  if ( temp == 9 ) // test the actual value returned by the function
    cout << "Pass" << endl;
  else
    cout << "Fail" << endl;

  cout << "TripleInt(0): ";
  temp = TripleInt(0);
  if ( temp == 0 ) // test the actual value returned by the function
    cout << "Pass" << endl;
  else
    cout << "Fail" << endl;

  cout << "TripleInt(-5): ";
  temp = TripleInt(-5);
  if ( temp == -15 ) // test the actual value returned by the function
    cout << "Pass" << endl;
  else
    cout << "Fail" << endl;

  cout << endl << "Testing SmallerInt" << endl;

  cout << "SmallerInt(3, 4): ";
  temp = SmallerInt(3, 4);
  if (temp == 3)
      cout << "Pass" << endl;
  else
      cout << "Fail" << endl;
  
  cout << "SmallerInt(4, 3): ";
  temp = SmallerInt(4, 3);
  if (temp == 3)
      cout << "Pass" << endl;
  else
      cout << "Fail" << endl;

  cout << "SmallerInt(3, 3): ";
  temp = SmallerInt(3, 3);
  if (temp == 3)
      cout << "Pass" << endl;
  else
      cout << "Fail" << endl;

  cout << "SmallerInt(-3, 3): ";
  temp = SmallerInt(-3, 3);
  if (temp == -3)
      cout << "Pass" << endl;
  else
      cout << "Fail" << endl;

  cout << endl << "Testing LargerInt" << endl;

  cout << "LargerInt(3, 4): ";
  temp = LargerInt(3, 4);
  if (temp == 4)
      cout << "Pass" << endl;
  else
      cout << "Fail" << endl;

  cout << "LargerInt(4, 3): ";
  temp = LargerInt(4, 3);
  if (temp == 4)
      cout << "Pass" << endl;
  else
      cout << "Fail" << endl;

  cout << "LargerInt(3, 3): ";
  temp = LargerInt(3, 3);
  if (temp == 3)
      cout << "Pass" << endl;
  else 
      cout << "Fail" << endl;

  cout << "LargerInt(-3, 3): ";
  temp = LargerInt(-3, 3);
  if (temp == 3)
     cout << "Pass" << endl;
  else
     cout << "Fail" << endl; 
  
  // continue testing the rest of your functions
  // test LargerInt 4 times with disparate values
  // test SmallerInt 4 times with disparate values
  


  return 0;
  }
