// INCLUDE THE LIBRARY FOR THE LOGGING FUNTIONS AND CREATE AND INSTANCE OF THE LOGGING STRUCT
#include "cmpslib19.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "Square.h"

int main()
{
	InitializeLogger("main3.log");


	Square a;
	cout << a.ToString() << endl;

	a.SetLength(5); 
	cout << a.ToString() << endl;

	Square b(10);
	cout << b.ToString() << endl;



	return  0;
}

