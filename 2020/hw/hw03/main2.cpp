#include "cmpslib19.h"


#include "functions.h"
#include "fraction.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP


int main()
{
	InitializeLogger("main2.log");


	// test that your functions work properly
	// make sure that they all work and do what they are supposed to do

	CFraction fData1[] ={ CFraction(1,2),CFraction(1,3),CFraction(1,4),CFraction(1,5),CFraction(2,5)
		,CFraction(3,2),CFraction(5,3),CFraction(3,4)};

	cout << "Testing with datatype fraction\n";
	// Print the Fraction Array
	PrintArray(fData1,8);

	// Fill the entire fraction array with random numbers using your custom function
	cout << "test FillArrayWithRandom\n";
	FillArrayWithRandom(fData1,8);
	// Print the Fraction Array
	PrintArray(fData1,8);

	

	// test sort ascending on fData1
	// use IsSortedAscending and IsSortedDescending in cmpslib.h
	cout << "\nTest sort SortAscending \n";
	SortAscending(fData1,8);
	cout << "IsSortedAscending returns:" << std::boolalpha << IsSortedAscending(fData1,8) << endl;
	cout << "IsSortedDescending returns:" << std::boolalpha << IsSortedDescending(fData1,8) << endl;
	PrintArray(fData1,8);


	

	// test sort descending on fData1
	cout << "\nTest sort SortDescending \n";
	SortDescending(fData1,8);
	cout << "IsSortedAscending returns:" << std::boolalpha << IsSortedAscending(fData1,8) << endl;
	cout << "IsSortedDescending returns:" << std::boolalpha << IsSortedDescending(fData1,8) << endl;
	PrintArray(fData1,8);

	

	// Test LargestInArray and SmallestInArray
	cout << "\nTest LargestInArray and SmallestInArray on fData1\n";
	cout << "\nLargest Value:" << LargestInArray(fData1,8) << endl;
	cout << "\nSmallest Value:" << SmallestInArray(fData1,8) << endl;

	cout << endl;
	return 0;
}





