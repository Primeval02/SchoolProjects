// INCLUDE THE LIBRARY FOR THE LOGGING FUNTIONS AND CREATE AND INSTANCE OF THE LOGGING STRUCT
#include "cmpslib19.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "Rectangular_Prism.h"

int main()
{
	InitializeLogger("main6.log");


	Rectangular_Prism a;
	cout << a.ToString() << endl;

	a.SetLength(5);
	a.SetWidth(7);  
	a.SetDepth(8);
	cout << a.ToString() << endl;

	Rectangular_Prism b(6,9,5);
	cout << b.ToString() << endl;



	return  0;
}

