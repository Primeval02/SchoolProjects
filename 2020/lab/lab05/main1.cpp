
#include "functions.h"
#include "cmpslib19.h"






int main()
{

  // since we are going to use a switch statement below lets create a few variables to use later 
  // we are not going to use any logging in this assignment  so dont bother opening a log

	bool finished = false;
	double d1;
	string s1;
	int int1,low,high, menu_choice;
	bool result;

	while (! finished)
	{  
// start your try block here (when you are ready to start catching exceptions

			/* 
				 prompt the user to enter a value then using if case statements 
				 perform the requested opertions 
				 DO THEM ALL WITHIN THHS ONE TRY BLOCK 
				 use the functions Prompt function in the cmpslib.h file for user input
				 
			 */
           
        try
        {
			Prompt("\nSelect Option (1-3)\n1. PrintFile\n2. Test StrToInt\n3. Test StrToDbl\n4. PrintFile (repeat till success)\n5. Test IsBetween\n6. Exit\n",menu_choice,1,6);
			switch( menu_choice )
			{
				case 1:
          // Prompt(string message_to_display, T & value_to_put_user_input); // the 2nd parmeter works for all types   
					Prompt("Enter Filename: ",s1);
					PrintFile(s1);
					break;

                case 2: 
                    Prompt("Enter string: ", s1);
                    int1 = StrToInt(s1);
                    cout << int1 << endl;
                    break;

					// string erase
                case 3:
                    Prompt("Enter a string: ", s1);
                    d1 = StrToDbl(s1);
                    cout << d1 << endl;
                    break;

                case 4:
                    result = true;
                    while(result)
                    {
                        try
                        {
                        Prompt("Enter filename: ", s1);
                        PrintFile(s1);
                        result = false;
                        }

                        catch(char const* value)
                        {
                        }
                    }

                case 5:
                    Prompt("Enter value: ", int1);
                    Prompt("Enter low: ", low);
                    Prompt("Enter high: ", high);
                    result = IsBetween(int1, low, high);
                    s1 = (result == 1) ? "true" : "false";
                    cout << s1 << endl;
                    break;

				case 6:
					finished = true;
					break;
			}

		cout   << "at the end of the try block , there must not have been any exceptions"  << endl;
		// end try when you add it
// put all the catch blocks here when you are ready
       }

       catch(std::string value)
       {
           cout << "Exception caught of type string, value = " << value << endl;
       }
       catch(std::runtime_error& value)
       {
           cout << "Exception caught of type std::runtime_error, value = " << value.what() << endl;
       }
       catch(int value)
       {
           cout << "Exception caught of type int, value = " << value << endl;
       }
       catch(const char* value)
       {
           cout << "Exception caught of type const char*, value = " << value << endl;
       }
       
	}; // end while

	return 0;
}








