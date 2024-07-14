#include "cmpslib19.h"
// INCLUDE THE LIBRARY FOR THE LOGGING FUNCTIONS AND THE MACRO TO INITIALIZE IT

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP


#include "Rectangular_Prism.h"
#include "Cube.h"


int main()
  {
  InitializeLogger("main2.log");


  Shape a;
  Line  b(5);
  Square c(5);
  Cube d(5);
  Rectangle e(5,10);
  Rectangular_Prism f(5,10,15);



  cout << a.ToString() << endl;
  cout << b.ToString() << endl;
  cout << c.ToString() << endl;
  cout << d.ToString() << endl;
  cout << e.ToString() << endl;
  cout << f.ToString() << endl;
  return  0;
  }

