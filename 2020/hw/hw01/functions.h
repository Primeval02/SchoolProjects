#include "easylogging++.h"
#include "cmpslib19.h"

// put your function bodies here


// create a function  that RETURNS the middle (not average) value of 3 integer parameters
int	Function1PBV( int a,int b,int c)
{
	LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "Start Params: " << a <<","<< b <<"."<< c << endl;
	// if (a < b  && a > c) then a is the middle
	// and so forth
    int result;
    if (a <= b && a >= c)
    {
        result = a;
    }

    if (a >= b && a <= c)
    {
        result = a;
    }

    if (b <= a && b >= c)
    {
        result = b;
    }

    if (b >= a && b <= c)
    {
        result = b;
    }

    if (c <= a && c >= b)
    {
        result = c;
    }

    if (c >= a && c <= b)
    {
        result = c;
    }

	LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "returning:" << result << endl;
	return result;
}

// create a function  that determines the middle value of ( int )parameters 2,3 & 4
// STORE THAT VALUE into the 1st parameter
void Function1PBR(int &result, int a,int b,int c)
{
	LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "Start Params: " << result << "," << a << "," << b << "." << c << endl;

	if (a <= b && a >= c)
    {
        result = a;
    }

    if (a >= b && a <= c)
    {
        result = a;
    }

    if (b <= a && b >= c)
    {
        result = b;
    }

    if (b >= a && b <= c)
    {
        result = b;
    }

    if (c <= a && c >= b)
    {
        result = c;
    }

    if (c >= a && c <= b)
    {
        result = c;
    }

	LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "End Params: " << result << "," << a << "," << b << "." << c << endl;

}




// create a function that returns the sum of parameters 1,2,3,4 & 5 
double Function2PBV(double a,double b,double c,double d,double e)
{
	LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "Start Params: " << a << "," << b << "." << c << "," << d << "," << e << endl; 
	double result = a + b + c + d + e;
	LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "Returning: " << result << endl; 
	return result;
}


// create a function that determines the sum of parameters 2,3,4,5 & 6
// STORE THAT VALUE into the 1st parameter
void	Function2PBR(double& result ,double a,double b,double c,double d,double e)
{
	LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "Start Params: " << result << "," << a << "," << b << "." << c << "," << d << "," << e << endl; 
	result = a + b + c + d + e;
	LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "End Params: " << result << "," << a << "," << b << "." << c << "," << d << "," << e << endl; 

}




// create a functions that can sort parameters 1 ,2 ,3  in ascending order
// use pass by refernce only for parameters that need to change
// regardles of the values in the parameter before calling
// after calling the 1st parameter will contain the lowest value
// after calling the 2nd parameter will contain the middle value
// after calling the 3rd parameter will contain the highest value

void Function3PBR(double& one , double& two, double& three)
{
	LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "Start Params: " <<  one << "," << two  << "." << three << endl; 
	// to sort
	// if one < two swap them
	// if two < three swap them
	// if one < two swap them
	// after those 3 operations it should be sorted
    double temp;
    if (one > two)
    {
        temp = one;
        one = two;
        two = temp;
    }

    if (two > three)
    {
        temp = two;
        two = three;
        three = temp;
    }

    if (one > two)
    {
        temp = one;
        one = two;
        two = temp;
    }

	
	LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "End Params: " <<  one << "," << two  << "." << three << endl; 
}







