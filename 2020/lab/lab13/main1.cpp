
#include "cmpslib19.h"
// INCLUDE THE LIBRARY FOR THE LOGGING FUNCTIONS AND THE MACRO TO INITIALIZE IT

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP


// include the standard template library stack

#include <stack>
#include "fraction.h"


// lets define ELEMTYPE  as the type we want our stack to hold

// we want to be sure it is defined defalut to be some known type


#ifndef ELEMTYPE  // only do the following define if its not already defined

#define ELEMTYPE  int
#endif


void ShowMenu()
{

	cout << "  mymystack Menu \n";
	cout << "  ===============================================\n";
    cout << "  i  Push a value into the stack\n";
    cout << "  t  Display the value returned by top()\n";
    cout << "  d  Pop a value from the stack\n";
    cout << "  e  Display the value returned by empty()\n";   
    cout << "  s  Display the value returned by size()\n";
	cout << "  m  Show this menu \n";
	cout << "  x  Exit \n";
	cout << "  ===============================================\n";

}


int main()
{
	InitializeLogger("example1.log");




	// create an stl::mystack

	// this is the only time you should use any stl containers

	std::stack<ELEMTYPE> mystack;
    ELEMTYPE temp;
	char selection;

	// Initialize the stack before the menu loop


	ShowMenu();
	do
	{
		// the Prompt function is defined in cmpslib19.h you can use it for any datatype

		// if you need to have the user input a value in one of the menu driven mains use this function instead of

		// using cin

		Prompt("Enter menu option: ",selection);
		cout << "\nOption " << selection << " chosen\n";

		switch(selection)
		{
            case 'i':
                Prompt("Enter element to Push into the stack: ", temp);
                mystack.push(temp);
                break;

            case 't':
                cout << "The top function returned " << mystack.top() << endl;
                break;

            case 'd':
                mystack.pop();
                break;

            case 'e':
                cout << boolalpha << mystack.empty() << endl;
                break;

            case 's':
                cout << mystack.size() << endl;
                break;
			case 'x':
				cout << "Goodbye.\n";
				break;

			case 'm':
				ShowMenu();
				break;
				
			default:
				cout << "Invalid menu option!\n";
		}
	}
	while(selection != 'x');

}


