// INCLUDE THE LIBRARY FOR THE LOGGING FUNTIONS AND CREATE AND INSTANCE OF THE LOGGING STRUCT
#include "cmpslib19.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "Cube.h"

int main()
{
	InitializeLogger("main4.log");

	Cube a;
	cout << a.ToString() << endl;

	a.SetLength(6);  
	cout << a.ToString() << endl;

	Cube b(11);
	cout << b.ToString() << endl;



	return  0;
}

