
#include "cmpslib19.h" // all the special functions provided for this class
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP



// put your funtions here
void TripleInt(int & val)
  {
  LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
  val *=3;
  LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
  }

void SmallerInt(int value1, int value2, int &smaller)
{
    LOG(INFO) << "Start: SmallerInt " << __PRETTY_FUNCTION__ << endl;
    smaller = (value1 <= value2) ? value1 : value2; 
    LOG(INFO) << "End: SmallerInt " << __PRETTY_FUNCTION__ << endl;
}

void LargerInt(int value1, int value2, int &larger)
{
    LOG(INFO) << "Start: LargerInt " << __PRETTY_FUNCTION__ << endl;
    larger = (value1 >= value2) ? value1 : value2;
    LOG(INFO) << "End: LargerInt " << __PRETTY_FUNCTION__ << endl;
}

void SwapInt(int &value1, int &value2)
{
    LOG(INFO) << "Start: SwapInt " << __PRETTY_FUNCTION__ << endl;
    int value3 = value1;
    value1 = value2;
    value2 = value3;
    LOG(INFO) << "End: SwapInt " << __PRETTY_FUNCTION__ << endl;
}

int main()
  {
  InitializeLogger("main1.log");



  cout << endl << endl;
  cout <<"Testing TripleInt" << endl;
  int temp=1;
  TripleInt(temp);
  cout << PassFail(3 == temp) << endl; // (3 == temp) will ONLY be true if the function worked properly

  temp = 3;
  TripleInt(temp);
  cout << PassFail(9 == temp) << endl;// (9 == temp) will ONLY be true if the function worked properly

  temp = 0;
  TripleInt(temp);
  cout << PassFail(0 == temp) << endl;// (0 == temp) will ONLY be true if the function worked properly

  temp = -5;
  TripleInt(temp);
  cout << PassFail(-15 == temp) << endl;// (-15 == temp) will ONLY be true if the function worked properly

  // continue testing the rest of your functions
  cout << endl << "Testing SmallerInt " << endl;

  temp = 0;
  SmallerInt(1, 2, temp);
  cout << PassFail (temp == 1) << endl;
 
  SmallerInt(2, 1, temp);
  cout << PassFail (temp == 1) << endl;

  SmallerInt(1, 1, temp);
  cout << PassFail (temp == 1) << endl;

  SmallerInt(1, -2, temp);
  cout << PassFail (temp == -2) << endl;

  cout << endl << "Testing LargerInt " << endl;

  LargerInt(1, 2, temp);
  cout << PassFail (temp == 2) << endl;

  LargerInt(2, 1, temp);
  cout << PassFail (temp == 2) << endl;

  LargerInt(2, 2, temp);
  cout << PassFail (temp == 2) << endl;

  LargerInt(-2, 1, temp);
  cout << PassFail (temp == 1) << endl;

  cout << endl << "Testing SwapInt " << endl;

  int value1 = 1;
  int value2 = 2;
  SwapInt(value1, value2);
  cout << PassFail (value2 == 1 && value1 == 2) << endl;

  value1 = 3;
  value2 = 4;
  SwapInt(value1, value2);
  cout << PassFail (value2 == 3 && value1 == 4) << endl;

  value1 = 5;
  value2 = 6;
  SwapInt(value1, value2);
  cout << PassFail (value2 == 5 && value1 == 6) << endl;

  value1 = 10;
  value2 = 20;
  SwapInt(value1, value2);
  cout << PassFail (value2 == 10 && value1 == 20) << endl;

  return 0;
  }
