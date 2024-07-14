#include <iostream>
using namespace std;
#include "cmpslib19.h"
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP


#define ELEMTYPE int
#define QUEUESIZE 4
#include "Queue.h"

int main()
{
  // set up the logger
  InitializeLogger("main1.log");


	Queue Queue1;

	try
	{
		// test that peekFront will throw an exception
		Queue1.peekFront();
		cout << "Fail:"<<endl;


	}
	catch (const char * val)
	{
		cout <<"Pass:"<<endl;
		cout <<"Exception:" << val << endl;
	}
	return 0;
}


