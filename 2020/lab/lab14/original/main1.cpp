#include "cmpslib19.h"

// INCLUDE THE LIBRARY FOR THE LOGGING FUNCTIONS AND THE MACRO TO INITIALIZE IT

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP





#include "Rectangular_Prism.h"
#include "Cube.h"


int main()
  {
  InitializeLogger("main1.log");

  Rectangular_Prism f(5,10,15);

  cout << f.ToString() << endl;
  return  0;
  }

