#include "cmpslib19.h"
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "DynamicArray.h"


int main()
  {
  InitializeLogger("main1.log");


  /*
  test all of the functions of your Dymamic array
  */


  DynamicArray  arr;
  DynamicArray  arr2;
  DynamicArray  arr3(7);




  cout << "Test == operator\n";

  cout <<" arr and arr2 are both empty, should be equal\n";
  cout << PF(    arr == arr2     ) << endl;

  cout <<" arr and arr2 are both empty, but different capacity  should NOT be equal\n";
  cout << PF(    (arr == arr3) == false      ) << endl;

  arr.SetValue("Homer",0);
  arr.SetValue("Marge",1);
  arr.SetValue("Bart",2);

  arr2.SetValue("Homer",0);
  arr2.SetValue("Marge",1);
  arr2.SetValue("Bart",2);


  cout <<"after adding Homer,Marge and Bart to both arr and arr2 should still be equal\n";
  cout << PF(    arr == arr2     ) << endl;


  arr.SetValue("Lisa",3);

  cout <<"after adding Lisa only to arr ,  arr and arr2 should NOT be equal\n";
  cout << PF(    (arr == arr2) == false     ) << endl;

  return 0;
  }

