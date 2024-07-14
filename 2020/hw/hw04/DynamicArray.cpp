#include "DynamicArray.h"


// put all the function bodies for the DynamicArray class here
DynamicArray::DynamicArray(int cap)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    capacity = cap;
    data = new string[capacity];
    LOG(INFO) << "End" << __PRETTY_FUNCTION__ << endl;
}

DynamicArray::DynamicArray(const DynamicArray & input)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    capacity = input.capacity;
    data = new string[capacity];
    for (int loop = 0; loop < capacity; loop++)
    {
        data[loop] = input.data[loop];
    }
    LOG(INFO) << "End" << __PRETTY_FUNCTION__ << endl;
}

DynamicArray::~DynamicArray()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    delete[] data;    
    LOG(INFO) << "End" << __PRETTY_FUNCTION__ << endl;
}

string DynamicArray::ToString()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    ostringstream temp;
    for (int loop = 0; loop < capacity; loop++)
    {
        temp << "data[" << loop << "] value: " << data[loop] << endl;
    }
    LOG(INFO) << "End" << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "End Returning " << temp.str() << endl;
    return temp.str();
}

bool DynamicArray::SetValue(string input, int pos)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    bool temp = true;
    if (pos < 0 || pos > capacity - 1)
    {
        temp = false;
    }
    else
    {
        data[pos] = input;
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "End Returning " << temp << endl;
    return temp;
}

