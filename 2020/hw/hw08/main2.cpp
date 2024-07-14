// INCLUDE THE LIBRARY FOR THE LOGGING FUNTIONS AND CREATE AND INSTANCE OF THE LOGGING STRUCT

#include "cmpslib19.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#ifndef ELEMTYPE
#define ELEMTYPE int
#endif

#include "Stack.h"


int main()
{
	// set up the logger 

	InitializeLogger("main2.log");// set logname





	Stack<ELEMTYPE> stack;
	int counter =100000;
	while (--counter > 0)
	{
		stack.Push(counter);
	}
	counter =100000;
	while (--counter > 5)
	{
		stack.Pop();
	}


	cout << stack.ToString() << endl;


}



