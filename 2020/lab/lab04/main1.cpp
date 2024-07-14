#include "cmpslib19.h"
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP


#include "fraction.h"
#include "template_functions.h"


int main()
  {
  InitializeLogger("main1.log");

  cout  << " Testing with type int"  << endl;

  cout  << " GetLarger( 2,3)  returns "  << GetLarger( 2,3) << endl;
  cout  << " GetLarger( 3,2)  returns "  << GetLarger( 3,2) << endl<< endl;

  cout  << " GetLarger( 1,2,3)  returns "  << GetLarger( 1,2,3) << endl;
  cout  << " GetLarger( 2,3,1)  returns "  << GetLarger( 2,3,1) << endl;
  cout  << " GetLarger( 3,2,1)  returns "  << GetLarger( 3,2,1) << endl<< endl;

  cout  << " GetSmaller(2,3)  returns "  << GetSmaller( 2,3) << endl;
  cout  << " GetSmaller( 3,2)  returns "  << GetSmaller( 3,2) << endl<< endl;

  cout  << " GetSmaller( 1,2,3)  returns "  << GetSmaller( 1,2,3) << endl;
  cout  << " GetSmaller( 2,3,1)  returns "  << GetSmaller( 2,3,1) << endl;
  cout  << " GetSmaller( 3,2,1)  returns "  << GetSmaller( 3,2,1) << endl<< endl;

    {
    int a=100,b=200;
    cout  << " a=" << a << ", b=" << b << endl;
    cout  << " SwapValues(a,b);" << endl;
    SwapValues(a,b);
    cout  << " a=" << a << ", b=" << b << endl;
    cout  << " SwapValues(a,b);" << endl;
    SwapValues(a,b);
    cout  << " a=" << a << ", b=" << b << endl;
    cout << endl;

    PromptForValue("\nEnter an int  value for a:",a);
    cout << "a now contains: " << a << endl;
    }


  LOG (INFO) << "\n\n\nTesting with type double" << endl;
  cout  << " Testing with type double"  << endl;

  cout  << " GetLarger( 2.2,3.3)  returns "  << GetLarger( 2.2,3.3) << endl;
  cout  << " GetLarger( 3.3,2.2)  returns "  << GetLarger( 3.3,2.2) << endl<< endl;

  cout  << " GetLarger( 1.1,2.2,3.3)  returns "  << GetLarger( 1.1,2.2,3.3) << endl;
  cout  << " GetLarger( 2.2,3.3,1.1)  returns "  << GetLarger( 2.2,3.3,1.1) << endl;
  cout  << " GetLarger( 3.3,2.2,1.1)  returns "  << GetLarger( 3.3,2.2,1.1) << endl<< endl;


  cout  << " GetSmaller(2.2,3.3)  returns "  << GetSmaller( 2.2,3.3) << endl;
  cout  << " GetSmaller( 3.3,2.2)  returns "  << GetSmaller( 3.3,2.2) << endl<< endl;

  cout  << " GetSmaller( 1.1,2.2,3.3)  returns "  << GetSmaller( 1.1,2.2,3.3) << endl;
  cout  << " GetSmaller( 2.2,3.3,1.1)  returns "  << GetSmaller( 2.2,3.3,1.1) << endl;
  cout  << " GetSmaller( 3.3,2.2,1.1)  returns "  << GetSmaller( 3.3,2.2,1.1) << endl<< endl;

    {
    double  a=10.1,b=20.2;
    cout  << " a=" << a << ", b=" << b << endl;
    cout  << " SwapValues(a,b);" << endl;
    SwapValues(a,b);
    cout  << " a=" << a << ", b=" << b << endl;
    cout  << " SwapValues(a,b);" << endl;
    SwapValues(a,b);
    cout  << " a=" << a << ", b=" << b << endl;
    cout << endl;
    PromptForValue("\nEnter a double value for a:",a);
    cout << "a now contains: " << a << endl;
    }


  LOG (INFO) << "\n\n\nTesting with type char" << endl;
  cout  << " Testing with type char"  << endl;
  cout  << " GetSmaller('a','b')  returns "  << GetSmaller('a','b') << endl;
  cout  << " GetSmaller('b','c')  returns "  << GetSmaller('b','c') << endl;
  cout  << " GetSmaller('c','a')  returns "  << GetSmaller('c','a') << endl<< endl;

  cout  << " GetSmaller('a','b','c')  returns "  << GetSmaller('a','b','c') << endl;
  cout  << " GetSmaller('b','c','a')  returns "  << GetSmaller('b','c','a') << endl;
  cout  << " GetSmaller('c','a','b')  returns "  << GetSmaller('c','a','b') << endl<< endl;

  cout  << " GetLarger('a','b')  returns "  << GetLarger('a','b') << endl;
  cout  << " GetLarger('b','c')  returns "  << GetLarger('b','c') << endl;
  cout  << " GetLarger('c','a')  returns "  << GetLarger('c','a') << endl<< endl;

  cout  << " GetLarger('a','b','c')  returns "  << GetLarger('a','b','c') << endl;
  cout  << " GetLarger('b','c','a')  returns "  << GetLarger('b','c','a') << endl;
  cout  << " GetLarger('c','a','b')  returns "  << GetLarger('c','a','b') << endl<< endl;

    {
    char a='L',b='M';
    cout  << " a=" << a << ", b=" << b << endl;
    cout  << " SwapValues(a,b);" << endl;
    SwapValues(a,b);
    cout  << " a=" << a << ", b=" << b << endl;
    cout  << " SwapValues(a,b);" << endl;
    SwapValues(a,b);
    cout  << " a=" << a << ", b=" << b << endl;
    cout << endl;
    PromptForValue("\nEnter a char value for a:",a);
    cout << "a now contains: " << a << endl;
    }

  LOG (INFO) << "\n\n\nTesting with type char *" << endl;
  cout  << " Testing with type char *"  << endl;

  cout  << " GetSmaller(\"monkey\",\"zebra\")  returns "  << GetSmaller( "monkey","zebra") << endl;
  cout  << " GetSmaller( \"zebra\",\"monkey\")  returns "  << GetSmaller( "zebra","monkey") << endl<< endl;

  cout  << " GetSmaller( \"capybara\",\"monkey\",\"zebra\")  returns "  << GetSmaller( "capybara","monkey","zebra") << endl;
  cout  << " GetSmaller( \"monkey\",\"zebra\",\"capybara\")  returns "  << GetSmaller( "monkey","zebra","capybara") << endl;
  cout  << " GetSmaller( \"zebra\",\"monkey\",\"capybara\")  returns "  << GetSmaller( "zebra","monkey","capybara") << endl<< endl;

  cout  << " GetLarger( \"monkey\",\"zebra\")  returns "  << GetLarger( "monkey","zebra") << endl;
  cout  << " GetLarger( \"zebra\",\"monkey\")  returns "  << GetLarger( "zebra","monkey") << endl<< endl;

  cout  << " GetLarger( \"capybara\",\"monkey\",\"zebra\")  returns "  << GetLarger( "capybara","monkey","zebra") << endl;
  cout  << " GetLarger( \"monkey\",\"zebra\",\"capybara\")  returns "  << GetLarger( "monkey","zebra","capybara") << endl;
  cout  << " GetLarger( \"zebra\",\"monkey\",\"capybara\")  returns "  << GetLarger( "zebra","monkey","capybara") << endl<< endl;



  LOG (INFO) << "\n\n\nTesting with type fraction" << endl;
  cout  << " Testing with type fraction"  << endl;

  cout  << " GetSmaller(5/3,2)  returns "   << GetSmaller( CFraction(5,3),CFraction(2) ) << endl;
  cout  << " GetSmaller( 2,5/3)  returns "  << GetSmaller( CFraction(2),CFraction(5,3) ) << endl<< endl;

  cout  << " GetSmaller( 1/2,2/2,3/2)  returns "  << GetSmaller( CFraction(1,2),CFraction(2,2),CFraction(3,2) ) << endl;
  cout  << " GetSmaller( 2/2,3/2,1/2)  returns "  << GetSmaller( CFraction(2,2),CFraction(3,2),CFraction(1,2) ) << endl;
  cout  << " GetSmaller( 3/2,1/2,2/2)  returns "  << GetSmaller( CFraction(3,2),CFraction(1,2),CFraction(2,2) ) << endl;

  cout  << " GetLarger( 1/2,1/3)  returns "  << GetLarger(  CFraction(1,2),CFraction(1,3) ) << endl;
  cout  << " GetLarger( 1/3,1/2)  returns "  << GetLarger(  CFraction(1,3),CFraction(1,2) ) << endl<< endl;

  cout  << " GetLarger( 1/2,2/2,3/2)  returns "  << GetLarger( CFraction(1,2),CFraction(2,2),CFraction(3,2) ) << endl;
  cout  << " GetLarger( 2/2,3/2,1/2)  returns "  << GetLarger( CFraction(2,2),CFraction(3,2),CFraction(1,2) ) << endl;
  cout  << " GetLarger( 3/2,1/2,2/2)  returns "  << GetLarger( CFraction(3,2),CFraction(1,2),CFraction(2,2) ) << endl << endl;

    {
    CFraction a(1,2), b(1,4);
    cout  << " a=" << a << ", b=" << b << endl;
    cout  << " SwapValues(a,b);" << endl;
    SwapValues(a,b);
    cout  << " a=" << a << ", b=" << b << endl;
    cout  << " SwapValues(a,b);" << endl;
    SwapValues(a,b);
    cout  << " a=" << a << ", b=" << b << endl;
    cout << endl;
    PromptForValue("Enter a fraction value for a:",a);
    cout << "a now contains: " << a << endl;
    }




  return 0;
  }
