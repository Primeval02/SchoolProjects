#include <iostream>
#include "cmpslib19.h"
#include "easylogging++.h"




// here are copies of GetLarger and GetSmaller for datatype char*
// you do NOT want to use the operators <,>,== because you will be comparing the pointers
const char * GetLarger (const char* a, const char* b)
  {
  LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
  const char *temp = (strcmp(a,b) <= 0) ? b:a;
  LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
  LOG(INFO) << "Returning " << temp << endl;
  return temp;
  }

const char * GetSmaller (const char* a, const char* b)
  {
  LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
  const char *temp = (strcmp(a,b) >= 0) ? b: a;
  LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
  LOG(INFO) << "Returning " << temp << endl;
  return temp;
  }


// T GetLarger (T a, T b)
// use the > operator to compare a & b
template <class T>
T GetLarger (T a, T b)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    T larger = (a >= b) ? a : b;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    return larger;
}

// T GetSmaller (T a, T b)
// use the < operator to compare a & b
template <class T>
T GetSmaller (T a, T b)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    T smaller = (a <= b) ? a : b;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    return smaller;
}

// T GetLarger (T a, T b, T c)
// log the value you will return like the examples
// DO NOT USE < or > in this function
template <class T>
T GetLarger (T a, T b, T c)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    T larger = GetLarger(a, b);
    larger = GetLarger(larger, c);  
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "Returning " << larger << endl;
    return larger;
}

// T GetSmaller (T a, T b, T c)
// log the value you will return like the examples
// DO NOT USE < or > in this function
template <class T>
T GetSmaller (T a, T b, T c)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    T smaller = GetSmaller(a, b);
    smaller = GetSmaller(smaller, c);
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "Returning " << smaller << endl;
    return smaller;
}


// void SwapValues( T & a, T & b)
// DO NOT use the "swap" function
template <class T>
void SwapValues (T & a, T & b)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    T c = a;
    a = b;
    b = c;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}


// void PromptForValue ( string message, T & val)
// use cin to read a value into val
template <class T>
void PromptForValue (string message, T & val)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    cout << message;
    cin >> val;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}


