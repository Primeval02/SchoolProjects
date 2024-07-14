#include "cmpslib19.h"
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "Square.h"
#include "Triangle.h"



int main()
{
  InitializeLogger("main1.log");

	//------------------------------------------------------------------


	LOG (INFO) << "testing the Square class \n";

	if (true)
	{

		// test Square class
        cout << "Create a Square named a, use the default constructor\na.";
        Square a;
        cout << a.ToString();
        cout << "\nUse the SetLength function to set a with the value -3.0\na.";
        a.SetLength(-3);
        cout << a.ToString();
        cout << "\nUse the SetLength function to set a width the value 3.5\na.";
        a.SetLength(3.5);
        cout << a.ToString();
        cout << "\nCreate a Square named b, use the paramaterized constructor to create one with the initial value of 44\nb.";
        Square b(44);
        cout << b.ToString();
	}

	LOG (INFO) << "finished testing the Square class \n\n\n\n";

	LOG (INFO) << "testing the Triangle class \n";
	if (true)
	{
        cout << "\nCreate a triangle named c, use the default constructor\nc.";
		Triangle c;
        cout << c.ToString();
        cout << "\nUse the SetLength funtion to set c with the value -3.0\nc.";
        c.SetLength(-3);
        cout << c.ToString();
        cout << "\nUse the SetLength function to set c with the value of 3.5\nc.";
        c.SetLength(3.5);
        cout << c.ToString();
		cout << "\nUse the SetHeight function to set c with the value -9\nc.";
        c.SetHeight(-9);
        cout << c.ToString();
        cout << "\nUse the SetHeight function to set c with the value 9.5\nc.";
        c.SetHeight(9.5);
        cout << c.ToString();
        cout << "\nCreate a Triangle named d, use the paramaterized constructor to create it with the initial length of 3 and a height of 9\nd.";
        Triangle d(3, 9);
        cout << d.ToString();

	}
	LOG (INFO) << "finished testing the Triangle class \n\n\n\n";



	//------------------------------------------------------------------


	return 0;
}
