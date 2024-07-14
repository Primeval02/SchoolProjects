#include "cmpslib19.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP


/* you should not need to modify this file at all */

#include "computer.h"

int main()
  {
	InitializeLogger("main1.log");


  Computer one;
  cout << one.ToString() << endl;
 
  Computer two(4,1.6 ,1000,4,"Pentium N3700");
  cout << two.ToString() << endl;


  Computer three( two );
  cout << three.ToString() << endl;


  return 0;
  }
