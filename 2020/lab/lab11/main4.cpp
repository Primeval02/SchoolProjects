#include "cmpslib19.h"
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "DynamicArray.h"


int main()
{
  InitializeLogger("main4.log");


	/*
		 test all of the functions of your Dymamic array
	 */


	DynamicArray  arr(3), arr2(8), arr3(2), arr4;

	cout << "test + operator  with DynamicArray of diffenent capacities \n";
	cout << "set arr and arr2  to contain some values \n";

	arr.SetValue("Homer",0);
	arr.SetValue("Marge",1);
	arr.SetValue("Bart",2);

	arr2.SetValue("Fry",0);
	arr2.SetValue("Leela",1);
	arr2.SetValue("Professor",2);
	arr2.SetValue("Zoiberg",3);
	arr2.SetValue("Scruffy",4);
	arr2.SetValue("Bender",5);
	arr2.SetValue("Amy",6);
	arr2.SetValue("Nibbler",7);

	arr3.SetValue("Rocky",0);
	arr3.SetValue("Bullwinkle",1);



	cout << "arr.ToString()\n" << arr.ToString() << endl;
	cout << "arr2.ToString()\n" << arr2.ToString() << endl;
	cout << "arr3.ToString()\n" << arr3.ToString() << endl;


	cout <<"arr4=arr+arr2;"<<endl;
	arr4=arr+arr2;
	cout << "arr4.ToString()\n" << arr4.ToString() << endl;

	cout <<"arr4=arr2+arr3;"<<endl;
	arr4=arr2+arr3;
	cout << "arr4.ToString()\n" << arr4.ToString() << endl;

	return 0;
}

