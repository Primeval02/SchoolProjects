// INCLUDE THE LIBRARY FOR THE LOGGING FUNTIONS AND CREATE AND INSTANCE OF THE LOGGING STRUCT
#include "cmpslib19.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "Line.h"

int main()
{
	InitializeLogger("main2.log");


	Shape a;
	cout << a.ToString() << endl;

	Line b;
	cout << b.ToString() << endl;

	b.SetLength(5);
	cout << b.ToString() << endl;

	Line c(9);
	cout << c.ToString() << endl;





	return  0;
}

