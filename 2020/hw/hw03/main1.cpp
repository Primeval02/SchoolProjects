#include "cmpslib19.h"


#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP



#include "functions.h"

int  main()
{
	InitializeLogger("main1.log");


	// test that your functions work properly
	// make sure that they all work and do what they are supposed to do

	int iaData1[10]={5,10,15,20,25,30,35,40,45,50};


	PrintArray(iaData1,10);
	// Test FillArrayWithRandom
    cout << "\nFilling the entire array with random values between 50 and 60 " << endl; 
// fill the entire array with random values between 50 and 60 
    FillArrayWithRandom(iaData1, 10, 50, 60);
// print it out with the function
    PrintArray(iaData1, 10);
// fill 2nd to 9th value of iaData1 with random values between -50 and 600
    cout << "\nFilling 2nd to 9th value of iaData1 with random values between 500 and 600" << endl;
    FillArrayWithRandom(iaData1 + 1, 9, 500, 600);
// print it out with the function
    PrintArray(iaData1, 10);

// Test LargestInArray and SmallestInArray
    cout << "\nTest LargestInArray and SmallestInArray on iaData1" << endl;
    cout << "Largest Value: " << LargestInArray(iaData1, 10) << endl;
    cout << "Smallest Value: " << SmallestInArray(iaData1, 10) << endl;
// test sort ascending on iaData1
    cout << "\nTest sort SortAscending " << endl;
    SortAscending(iaData1, 10);  
// use IsSortedAscending and IsSortedDescending in cmpslib.h

	cout << "IsSortedAscending returns:" << std::boolalpha << IsSortedAscending(iaData1,10) << endl;
	cout << "IsSortedDescending returns:" << std::boolalpha << IsSortedDescending(iaData1,10) << endl;
	


	

	// test sort descending on iaData1 
	cout << "\nTest sort SortDescending \n";
    SortDescending(iaData1,10);
	
	cout << "IsSortedAscending returns:" << std::boolalpha << IsSortedAscending(iaData1,10) << endl;
	cout << "IsSortedDescending returns:" << std::boolalpha << IsSortedDescending(iaData1,10) << endl;
	




	cout  << endl;
	
	return 0;
}





