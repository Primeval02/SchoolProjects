#pragma once
#include "cmpslib19.h"
#include "easylogging++.h"


class DynamicArray
  {
    // private data members
    private:
        string * data;
        int capacity;
        static int object_count;
//publc functions
    public: 
        DynamicArray(int cap = 5);
        DynamicArray(const DynamicArray & input);
        DynamicArray(DynamicArray & input);
        ~DynamicArray();
        string ToString();
        bool SetValue(string input, int pos);
        static int GetObjectCount();
        friend string CreateXML(DynamicArray & object);

  };
