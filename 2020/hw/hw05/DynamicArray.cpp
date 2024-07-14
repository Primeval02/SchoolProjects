#include "DynamicArray.h"


// put all the function bodies for the DynamicArray class here

int DynamicArray::object_count = 0;

DynamicArray::DynamicArray(int cap)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    object_count++;
    capacity = cap;
    data = new string[capacity];
    LOG(INFO) << "End" << __PRETTY_FUNCTION__ << endl;
}

DynamicArray::DynamicArray(const DynamicArray & input)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    object_count++;
    capacity = input.capacity;
    data = new string[capacity];
    for (int loop = 0; loop < capacity; loop++)
    {
        data[loop] = input.data[loop];
    }
    LOG(INFO) << "End" << __PRETTY_FUNCTION__ << endl;
}

DynamicArray::DynamicArray(DynamicArray & input)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    object_count++;
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
    object_count--;
    delete[] data;    
    LOG(INFO) << "End" << __PRETTY_FUNCTION__ << endl;
}

string DynamicArray::ToString()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    ostringstream temp;
    cout << "capacity: " << capacity << endl;
    cout << "object_count: " << object_count << endl;
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

int DynamicArray::GetObjectCount()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    return object_count;
}

string CreateXML(DynamicArray & object)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    ostringstream temp;
    temp << "<DynamicArray>" << endl;
    temp << "<capacity>" << object.capacity << "</capacity>" << endl;
    temp << "<object_count>" << object.object_count << "</object_count>" << endl;
    temp << "<data>" << endl;
    for (int loop = 0; loop < object.capacity; loop++)
    {
        temp << "  " << "<value>" << object.data[loop] << "</value>" << endl;
    }
    temp << "</data>" << endl;
    temp << "</DynamicArray>" << endl;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    return temp.str();
}
