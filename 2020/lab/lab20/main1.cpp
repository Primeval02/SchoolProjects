
#include "cmpslib19.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP


#ifndef ELEMTYPE
#define ELEMTYPE int
#endif

#include "DLinkedList.h"
#include "fraction.h"


void ShowMenu()
{
	cout << "  Doubly Linked List Menu \n";
	cout << "  ==================================================\n";
	cout << "  i  Insert a value into the list                   \n";
    cout << "  a  Insert a value after a value                   \n";
	cout << "  p  Print the value returned by ToString()         \n";
	cout << "  m  Show this menu                                 \n";
	cout << "  x  Exit                                        \n";
	cout << "  ==================================================\n";
}

int main()
{
	InitializeLogger("example1.log");



	if (true)
	{ // make a subsccope


		DLinkedList<ELEMTYPE>  list;
		ELEMTYPE elem;
        ELEMTYPE after;

		char selection;
		ShowMenu();

		do
		{
			Prompt("  Enter selection: ",selection);
			cout << "\nOption " << selection << " chosen\n";
            try
            {
			    switch(selection)
			    {
				    case 'x':
					    cout << "Goodbye.\n";
					    break;
				    case 'm':
					    ShowMenu();
					    break;

				    case 'i':
					    Prompt("Enter element to insert into the list: ",elem);
					    if(list.Insert(elem))
					    {
						    cout << elem << " was added.\n";
					    }
					    else
					    {
						    cout << elem << " was NOT added.\n";
					    }
					    break;

                    case 'a':
                        Prompt("Enter element to insert into the list: ", elem);
                        Prompt("Enter element to insert after: ", after);
                        if(list.InsertAfter(elem, after))
                        {
                            cout << elem << " was added.\n";
                        }
                        else
                        {
                            cout << elem << " was NOT added.\n";
                        }
                        break;

				    case 'p':
					    cout << list.ToString() << endl;
					    break;

                   /* case 'd':
                        Prompt("Enter element to delete from list: ", elem);
                        if (list.Delete(elem))
                        {
                            cout << elem << " was removed from the list.\n";
                        }
                        else
                        {
                            cout << elem << " was unable to be removed.\n";
                        }
                        break;

                    case 'e':
                        if(list.Delete(list.Back()))
                        {
                            cout << "Last value was removed from the list.\n";
                        }
                        else
                        {
                            cout << "unable to remove the last value.\n";
                        }
                        break;

                    case 'c':
                        Prompt("Enter a value to check for: ", elem);
                        if (list.Contains(elem))
                        {
                            cout << elem << " is in the list.\n";
                        }
                        else
                        {
                            cout << elem << " is not in the list.\n";
                        }
                        break;

                    case 'f':
                        cout << list.Front() << endl;
                        break;

                    case 'b':
                        cout << list.Back() << endl;
                        break; */

				    default:
					    cout << "Invalid menu option!\n";
                }
			}

            catch (const char * val)
            {
                cout << "Exception Caught: " << val << endl;
            }

		}
		while(selection != 'x');
	}// end of subscope

}



