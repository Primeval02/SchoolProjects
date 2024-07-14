#include "cmpslib19.h"
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "DynamicArray.h"


int main()
{
  InitializeLogger("main2.log");


	/*
		 test all of the functions of your Dymamic array
	 */


	DynamicArray  arr(3), arr2(8);

	cout << "test = assignment operator with DynamicArray of same capacity\n";
	cout << "set arr and arr2  to contain some values \n";

	arr.SetValue("Homer",0);
	arr.SetValue("Marge",1);
	arr.SetValue("Bart",2);

	arr2.SetValue("Fry",0);
	arr2.SetValue("Leela",1);
	arr2.SetValue("Professor",2);

	cout << "arr.ToString()\n" << arr.ToString() << endl;
	cout << "arr2.ToString()\n" << arr2.ToString() << endl;
	cout <<"arr=arr2;"<<endl;
	arr=arr2;
	cout << "arr.ToString()\n" << arr.ToString() << endl;

	return 0;
}

