#include "cmpslib19.h"
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "DynamicArray.h"


int main()
{
	InitializeLogger("main3.log");



	cout << "Test Default Consructor" << endl;
	DynamicArray arr1;
	if(
			arr1.SetValue("one",0) && 
			arr1.SetValue("two",1) && 
			arr1.SetValue("three",2) && 
			arr1.SetValue("four",3) && 
			arr1.SetValue("five",4) && 
			(false == arr1.SetValue("six",5))
		) 
		cout << "Passed\n" ;
	else 
		cout << "Failed\n";
	cout << arr1.ToString() << endl;

	cout << "Test Consructor with a value" << endl;
	DynamicArray arr2(4);
	if(
			arr2.SetValue("one",0) && 
			arr2.SetValue("two",1) && 
			arr2.SetValue("three",2) && 
			arr2.SetValue("four",3) && 
			(false == arr2.SetValue("four",5))
		) 
		cout << "Passed\n" ;
	else 
		cout << "Failed\n";
	cout << arr2.ToString() << endl;


	{
		cout << "\n\nCreate a very large DynamicArray\n" ;
		DynamicArray arr2(10000);
		for(auto loop =0;loop < 10000;loop++)
			if (false==	arr2.SetValue("Chubbs the Wampug",loop))
			{
				cout << "Failed on insert.\n";
				break;
			}
		// insert one more SHOULD fail this time  
		if (true==	arr2.SetValue("Chubbs the Wampug",10000))
		{
			cout << "allowed out of bounds insert\n";
		}
	}

	cout << "\n\nAfter Create a very large DynamicArray\n" ;


	return 0;
}

