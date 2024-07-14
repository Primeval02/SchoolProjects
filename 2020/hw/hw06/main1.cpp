// INCLUDE THE LIBRARY FOR THE LOGGING FUNTIONS AND CREATE AND INSTANCE OF THE LOGGING STRUCT
#include "cmpslib19.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP


#include "Shape.h"

int main()
{
	InitializeLogger("main1.log");


	Shape a;
	cout << a.ToString() << endl;
	return  0;
}

