#include "cmpslib19.h"
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "DynamicArray.h"


int main()
{
	InitializeLogger("main2.log");


	DynamicArray  arr;

	if (true  == arr.SetValue("Homer",-1) || true == arr.SetValue("Homer",5))
		cout << "FAIL, out of bounds Set is allowed." << endl;
	else
		cout << "Pass, bounds testing seems ok." << endl;





	bool result;
	/* 
		 the Dynamic array better have a default size of 5
		 Apu should NOT get entered but the rest should
	 */


	cout << "arr.ToString() returns " << endl << arr.ToString() << endl;

	result  =arr.SetValue("Homer",0);
	cout << "arr.SetValue(\"Homer\",0) returns " << std::boolalpha << result << endl;    

	result  =arr.SetValue("Marge",1);
	cout << "arr.SetValue(\"Marge\",1) returns " << std::boolalpha << result << endl;    

	result  =arr.SetValue("Bart",2);
	cout << "arr.SetValue(\"Bart\",2) returns " << std::boolalpha << result << endl;    

	result  =arr.SetValue("Lisa",3);
	cout << "arr.SetValue(\"Lisa\",3) returns " << std::boolalpha << result << endl;    

	result  =arr.SetValue("Maggie",4);
	cout << "arr.SetValue(\"Maggie\",4) returns " << std::boolalpha << result << endl;    

	result  =arr.SetValue("Apu",5);
	cout << "arr.SetValue(\"Apu\",5) returns " << std::boolalpha << result << endl;    

	if (result == true)
		cout << RED("Fail, the default size should not allow this insert ") << endl;

	cout << endl;

	cout << "arr.ToString() returns " << endl << arr.ToString() << endl;

	result  =arr.SetValue("Manjula",0);
	cout << "arr.SetValue(\"Manjula\",0) returns " << std::boolalpha << result << endl;    

	cout << "arr.ToString() returns " << endl << arr.ToString() << endl ;

	cout << "Test the copy constructor\n";

	DynamicArray arr2(arr);

	arr.SetValue("Better not be in arr2",0);
	arr.SetValue("Better not be in arr2 either",1);

	cout << "arr2.ToString() returns " << endl << arr.ToString() << endl ;
	cout << arr2.ToString() << endl;

	return 0;
}

