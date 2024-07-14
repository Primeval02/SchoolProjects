#include "cmpslib19.h"


#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP



#include "functions.h"

int main()
{
  InitializeLogger("main1.log");
  LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
	// test that your functions work properly
	// make sure that they all work and do what they are supposed to do

	int a;
	double b;
	bool result;
    string temp;

	cout << "\n\nTesting function1 with int\n";
	try
	{
		cout << "Difference(25,20): ";
		a = Difference(25,20);
		cout << a  << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}

	try
	{
		cout << "Difference(25,30): ";
		a = Difference(25,30);
		cout << a  << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}


	cout << "\n\nTesting function1 with double\n";
	try
	{
		cout << "Difference(2.5,2.0): ";
		b = Difference(2.5,2.0);
		cout << b  << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}

	try
	{
		cout << "Difference(2.5,3.0): ";
		b = Difference(2.5,3.0);
		cout << b  << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}


 	cout << "\n\nTesting function2 with int\n";
	try
	{
		cout << "Difference(25,20): ";
		cout << Difference(25,20) << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}

	try
	{
		cout << "Difference(25,30): ";
		cout << Difference(25,30) << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}


	cout << "\n\nTesting function2 with double\n";
	try
	{
		cout << "Difference(2.5,2.0): ";
		cout << Difference(2.5,2.0) << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}

	try
	{
		cout << "Difference(2.5,3.0): ";
		cout << Difference(2.5,3.0) << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}


	cout << "\n\nTesting function3 with int\n";
	try
	{
		cout << "IsBetween(2, 1, 4): ";
		result = IsBetween(2, 1, 4);
        if (result == true)
        {
            temp = "true";
        }
        else
        {
            temp = "false";
        }
		cout << temp << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}

	try
	{
		cout << "IsBetween(3, 7, 2): ";
		result = IsBetween(3, 7, 2);
        if (result == true)
        {
            temp = "true";
        }
        else
        {
            temp = "false";
        }
		cout << temp << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}


	cout << "\n\nTesting function3 with double\n";
	try
	{
		cout << "IsBetween(2.5, 1.0, 4.5): ";
		result = IsBetween(2.5, 1.0, 4.5);
        if (result == true)
        {
            temp = "true";
        }
        else
        {
            temp = "false";
        }
		cout << temp << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}

	try
	{
		cout << "IsBetween(3.0, 7.5, 2.0): ";
		result = IsBetween(3.0, 7.5, 2.0);
        if (result == true)
        {
            temp = "true";
        }
        else
        {
            temp = "false";
        }
		cout << temp << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}


 	cout << "\n\nTesting function4 with int\n";
	try
	{
		cout << "IsBetween(2, 1, 4): ";
		result = IsBetween(2, 1, 4);
        if (result == true)
        {
            temp = "true";
        }
        else
        {
            temp = "false";
        }
        cout << temp << endl;

	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}

	try
	{
		cout << "IsBetween(3, 7, 2): ";
		result = IsBetween(3, 7, 2);
        if (result == true)
        {
            temp = "true";
        }
        else
        {
            temp = "false";
        }
        cout << temp << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}


	cout << "\n\nTesting function4 with double\n";
	try
	{
		cout << "IsBetween(2.5, 1.0, 4.5): ";
		result = IsBetween(2.5, 1.0, 4.5);
        if (result == true)
        {
            temp = "true";
        }
        else
        {
            temp = "false";
        }
        cout << temp << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}

	try
	{
		cout << "IsBetween(3.0, 7.5, 2.0): ";
		result = IsBetween(3.0, 7.5, 2.0);
        if (result == true)
        {
            temp = "true";
        }
        else
        {
            temp = "false";
        }
        cout << temp << endl;
	}
	catch (const char * val)
	{   cout << "\nException:" << val << endl;}
       
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
	return 0;
}
