#include "cmpslib19.h" // all the special functions provided for use in this class


#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP



#include "functions.h"

int main()
{
	InitializeLogger("main1.log");


	int iResult;
	double dResult;

	std::cout << "\nTesting Function1PBV\n";
	iResult = Function1PBV(0,-10,10);// does it work when param1 is the middle value
	if (0 == iResult) // always put the constant value on the left hand side of == operator, best practice
		std::cout << "Pass" << endl;
	else 
		std::cout << "Fail" << endl;

	// the library ( in cmpslib.h ) function PF has a bool parmmeter and returns the c string "Pass" or "Fail" 
	// so this is the same code but shorter using the funtion PF
	iResult = Function1PBV(-50,-50,10);// does it work when param2 is the middle value
	std::cout <<  PF(-50 ==  iResult ) << endl;      // consant value on left of == NOT (iResult == -10)
	// test this function 2 more times
    iResult = Function1PBV(2, 3, 4);
    std::cout << PF(3 == iResult) << endl;

    iResult = Function1PBV(23, 4, 7);
    std::cout << PF(7 == iResult) << endl;

// does it work when param3 is the middle value
// what if 2 are the same
// what if 2 are the same

	std::cout << "\nTesting Function1PBR\n";
	Function1PBR(iResult,-10,10,3);
	std::cout << PF(3 == iResult ) << endl;// 3 is the ONLY correct answer
	// test this function 3 more times
    Function1PBR(iResult, 2, 3, 4);
    std::cout << PF(3 == iResult) << endl;

    Function1PBR(iResult, 23, 4, 7);
    std::cout << PF(7 == iResult) << endl;

    Function1PBR(iResult, 9, 1, 6);
    std::cout << PF(6 == iResult) << endl;


	// Function2 has double parmeters instead of integers
	// comparing doubles can be problematic so there is a helper function in the cmpslib19.h 
	// that you should use

	std::cout << "\nTesting Function2PBV\n";
	dResult = Function2PBV(10.1,100.1,10.1,-1.10,100.1);
	std::cout << "NOTE: the function should return 219.3 and it appears to return " <<  dResult <<  endl;	
	std::cout <<  PF(219.3 == dResult)  << " ,this is why we dont want to use == to compare doubles here" <<  endl; // this will say fail even if the functtion returns 219.3
	std::cout <<  PF(VeryClose(219.3,dResult))  << " ,this is using the VeryClose function" <<  endl; // so to compare doubles use the function   "bool VeryClose(double, double)"

	// test this function 3 more times using VeryClose
    dResult = Function2PBV(20.3, 30.1, 99.9, 7.7, 90.2);
    std::cout << PF(VeryClose(248.2, dResult)) << endl;

    dResult = Function2PBV(11.1, 8.8, 4.6, 32.6, 90.3);
    std::cout << PF(VeryClose(147.4, dResult)) << endl;

    dResult = Function2PBV(10.1, 20.2, 30.3, 40.4, 50.5);
    std::cout << PF(VeryClose(151.5, dResult)) << endl;


	std::cout << "\nTesting Function2PBR \n";
	Function2PBR(dResult,100.1,100.1,100.1,100.1,100.1);
	std::cout <<  PF(VeryClose(500.5,dResult) ) << endl;
	// test function 3 more times
    Function2PBR(dResult, 10.1, 20.2, 30.3, 40.4, 50.5);
    std::cout << PF(VeryClose(151.5, dResult)) << endl;

    Function2PBR(dResult, 11.1, 8.8, 4.6, 32.6, 90.3);
    std::cout << PF(VeryClose(147.4, dResult)) << endl;

    Function2PBR(dResult, 20.3, 30.1, 99.9, 7.7, 90.2);
    std::cout << PF(VeryClose(248.2, dResult)) << endl;



	double c =100.1;
	double b =200.2;
	double a =300.3;
	std::cout << "\nTesting Function3PBR \n";
	Function3PBR(a,b,c);
	// test them to make sure they are sorted now
	std::cout << PF( VeryClose(100.1,a)  && VeryClose( 200.2 ,b)  && VeryClose(300.3,c)  ) << endl; 
	//test function 2 more times
    c = 1.1;
    b = 22.2;
    a = 500.5;
    Function3PBR(a, b, c);
    std::cout << PF(VeryClose(1.1, a) && VeryClose(22.2, b) && VeryClose(500.5, c)) << endl;

    c = 1.1;
    b = 2.2;
    a = 3.3;
    Function3PBR(a, b, c);
    std::cout << PF(VeryClose(1.1, a) && VeryClose(2.2, b) && VeryClose(3.3, c)) << endl;





	return 0;
}







