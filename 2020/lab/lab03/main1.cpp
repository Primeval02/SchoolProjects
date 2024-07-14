
#include "cmpslib19.h" // all the special functions provided for this class

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP



#include "functions.h"


int main()
{
	InitializeLogger("main1.log");
    LOG(INFO) << "Start: Main " << __PRETTY_FUNCTION__ << endl;

	/*
		 test all your functions here
	 */


	cout << endl << endl;
	cout << endl;
	cout <<"Testing TripleInt pbv" << endl;
	cout << PassFail(0 == TripleIntPBV(0)      ) << endl;
	cout << PassFail(5 != TripleIntPBV(5)      ) << endl;
	cout << PassFail(15 == TripleIntPBV( 5 )   ) << endl;
	cout << PassFail(-15 == TripleIntPBV( -5 ) ) << endl;
	cout << PassFail(15 == TripleIntPBV( 5.5 ) ) << endl;

	cout <<"Testing TripleInt pbp" << endl;
	int temp =0;
	TripleIntPBP(&temp);
	cout << PassFail( 0 == temp ) << endl;
	temp = -5;
	TripleIntPBP(&temp);
	cout << PassFail(-15 == temp  ) << endl;
	temp = 5;
	TripleIntPBP(&temp);
	cout << PassFail( 15 == temp )  << endl;
	temp=5.5;
	TripleIntPBP(&temp);
	cout << PassFail( 15 == temp )  << endl;

	// continue testing the rest of your functions
    cout << "Testing TripleIntPBR" << endl;
    temp = 0;
    TripleIntPBR(temp);
    cout << PassFail(0 == temp) << endl;
    temp = 5;
    TripleIntPBR(temp);
    cout << PassFail(15 == temp) << endl;
    temp = -5;
    TripleIntPBR(temp);
    cout << PassFail(-15 == temp) << endl;
    temp = 4;
    TripleIntPBR(temp);
    cout << PassFail(12 == temp) << endl;    

    cout << "Testing SwapIntPBP" << endl;
    int value1 = 1;
    int value2 = 2;
    SwapIntPBP(&value1, &value2);
    cout << PassFail(value2 == 1 && value1 == 2) << endl;
    value1 = 3;
    value2 = 4;
    SwapIntPBP(&value1, &value2);
    cout << PassFail(value2 == 3 && value1 == 4) << endl;
    value1 = 7;
    value2 = 9;
    SwapIntPBP(&value1, &value2);
    cout << PassFail(value2 == 7 && value1 == 9) << endl;
    value1 = 3;
    value2 = 6;
    SwapIntPBP(&value1, &value2);
    cout << PassFail(value2 == 3 && value1 == 6) << endl;

    cout << "Testing SwapIntPBR" << endl;
    value1 = 1;
    value2 = 2;
    SwapIntPBR(value1, value2);
    cout << PassFail(value2 == 1 && value1 == 2) << endl;
    value1 = -3;
    value2 = 5;
    SwapIntPBR(value1, value2);
    cout << PassFail(value2 == -3 && value1 == 5) << endl;
    value1 = 4;
    value2 = 7;
    SwapIntPBR(value1, value2);
    cout << PassFail(value2 == 4 && value1 == 7) << endl;
    value1 = 3;
    value2 = 0;
    SwapIntPBR(value1, value2);
    cout << PassFail(value2 == 3 && value1 == 0) << endl;

    cout << "Testing LargerInt" << endl;
    temp = LargerInt(1, 2);
    cout << PassFail(2 == temp) << endl;
    temp = LargerInt(3, 4);
    cout << PassFail(4 == temp) << endl;
    temp = LargerInt(7, 9);
    cout << PassFail(9 == temp) << endl;
    temp = LargerInt(-4, 1);
    cout << PassFail(1 == temp) << endl;

    cout << "Testing LargerIntPBR" << endl;
    LargerIntPBR(4, 7, temp);
    cout << PassFail(7 == temp) << endl;
    LargerIntPBR(5, -10, temp);
    cout << PassFail(5 == temp) << endl;
    LargerIntPBR(1, 2, temp);
    cout << PassFail(2 == temp) << endl;
    LargerIntPBR(2, 1, temp);
    cout << PassFail(2 == temp) << endl;

    cout << "Testing LargerIntPBP" << endl;
    int temp2;
    value1 = 1;
    value2 = 2;
    LargerIntPBP(value1, value2, &temp2);
    cout << PassFail(2 == temp2) << endl;
    value1 = 4;
    value2 = 3;
    LargerIntPBP(value1, value2, &temp2);
    cout << PassFail(4 == temp2) << endl;
    value1 = 7;
    value2 = 2;
    LargerIntPBP(value1, value2, &temp2);
    cout << PassFail(7 == temp2) << endl;
    value1 = 3;
    value2 = 9;
    LargerIntPBP(value1, value2, &temp2);
    cout << PassFail(9 == temp2) << endl;

    LOG(INFO) << "End: Main " << __PRETTY_FUNCTION__ << endl;
	return 0;
}
