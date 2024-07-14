#pragma once
#include "cmpslib19.h"
#include "easylogging++.h"


// this is the complete class declaration

class StaticArray
  {

  private:
    string data[5];

  public:
    StaticArray();
    ~StaticArray();
    string ToString();
    bool SetValue(string,int);

  };
