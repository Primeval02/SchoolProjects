
#include "cmpslib19.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#ifndef ELEMTYPE  // only do the following define if its not already defined
#define ELEMTYPE  int
#endif

#include "Stack.h"


int main()
{

   // set up the logger
   el::Configurations LGR;
   LGR.setToDefault();// load current configuration
   LGR.setGlobally(el::ConfigurationType::Filename,"main2.log");// set logname
   LGR.set(el::Level::Info,el::ConfigurationType::ToStandardOutput,"false");// dont push Info to console
   LGR.setGlobally(el::ConfigurationType::LogFlushThreshold,"1");// start a new log file very 1 time
   el::Loggers::reconfigureLogger("default", LGR);// store new configuration


	Stack stack;
	for(int loop=0;loop<6;loop++)
	{ 
		cout << std::boolalpha << "adding "<< loop  << stack.Push(loop) << endl;
	}

}



