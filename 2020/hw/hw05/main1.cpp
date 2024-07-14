#include "cmpslib19.h" // all the special functions provided for use in this class
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "DynamicArray.h"

int main()
{
   // set up the logger 
   el::Configurations LGR;
   LGR.setToDefault();// load current configuration
   LGR.setGlobally(el::ConfigurationType::Filename,"main1.log");// set logname
   LGR.set(el::Level::Info,el::ConfigurationType::ToStandardOutput,"false");// dont push Info to console
   LGR.setGlobally(el::ConfigurationType::LogFlushThreshold,"1");// start a new log file very 1 time
   el::Loggers::reconfigureLogger("default", LGR);// store new configuration

  if (true)
    {
    cout << "GetObjectCount returns" << DynamicArray::GetObjectCount() << endl;
    DynamicArray  arr;

    bool result;

    cout << "arr.ToString() returns " << endl << arr.ToString() << endl;

    result  =arr.SetValue("Homer",0);
    cout << "arr.SetValue(\"Homer\",0) returns " << std::boolalpha << result << endl;

    result  =arr.SetValue("Marge",1);
    cout << "arr.SetValue(\"Marge\",1) returns " << std::boolalpha << result << endl;

    result  =arr.SetValue("Bart",2);
    cout << "arr.SetValue(\"Bart\",2) returns " << std::boolalpha << result << endl;

    result  =arr.SetValue("Lisa",3);
    cout << "arr.SetValue(\"Lisa\",3) returns " << std::boolalpha << result << endl;


    result  =arr.SetValue("Maggie",4);
    cout << "arr.SetValue(\"Maggie\",4) returns " << std::boolalpha << result << endl;

    result  =arr.SetValue("Apu",5);
    cout << "arr.SetValue(\"Apu\",5) returns " << std::boolalpha << result << endl;

    result  =arr.SetValue("Manjula",6);
    cout << "arr.SetValue(\"Manjula\",6) returns " << std::boolalpha << result << endl;

    cout << "arr.ToString() returns " << endl << arr.ToString() << endl ;

 
    cout << "arr as XML:" << endl << CreateXML(arr) << endl;

    cout << "GetObjectCount returns" << DynamicArray::GetObjectCount() << endl;
    }
    cout << "GetObjectCount returns" << DynamicArray::GetObjectCount() << endl;
  return 0;
  }

