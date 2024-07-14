// INCLUDE THE LIBRARY FOR THE LOGGING FUNTIONS AND CREATE AND INSTANCE OF THE LOGGING STRUCT
#include "cmpslib19.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "Rectangle.h"

int main()
{
	InitializeLogger("main5.log");


	Rectangle a;
	cout << a.ToString() << endl;

	a.SetLength(5);
	a.SetWidth(7);  
	cout << a.ToString() << endl;

	Rectangle b(6,9);
	cout << b.ToString() << endl;



	return  0;
}

