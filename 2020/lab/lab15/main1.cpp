#include "cmpslib19.h"

// INCLUDE THE LIBRARY FOR THE LOGGING FUNCTIONS AND THE MACRO TO INITIALIZE IT

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP



#include "fraction.h"
#include "template_class.h"


int main()
  {
	InitializeLogger("main1.log");

    cout << "Testing with type int " << endl;
    demo<int> a;
    a.setData(99);
    cout << "a.setData(99)" << endl;
    a.getData();
    cout << "a.getData() returns: 99 " << endl;
    cout << "cout << a" << endl;
    cout << a << endl;

    cout << "Testing with type double " << endl;
    demo<double> b;
    b.setData(99.99);
    cout << "b.setData(99.99)" << endl;
    b.getData();
    cout << "b.getData() returns: 99.99 " << endl;
    cout << "cout << b" << endl;
    cout << b << endl;

    cout << "Testing with type string " << endl;
    demo<string> c;
    c.setData("monkey");
    cout << "c.setData(\"monkey\")" << endl;
    a.getData();
    cout << "c.getData() returns: monkey " << endl;
    cout << "cout << c" << endl;
    cout << c << endl;

    cout << "Testing with type fraction " << endl;
    demo<CFraction> d;
    d.setData(CFraction(1, 3));
    cout << "d.setData(CFraction(1, 3))" << endl;
    d.getData();
    cout << "d.getData() returns: 1/3 " << endl;
    cout << "cout << d" << endl;
    cout << d << endl;




  

  return 0;
  }
