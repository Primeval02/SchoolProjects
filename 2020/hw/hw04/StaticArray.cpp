#include "StaticArray.h"


StaticArray::StaticArray()
{
	LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
	// nothing to do
	LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
}

StaticArray::~StaticArray()
{
	LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
	// nothing to do
	LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;

}

string StaticArray::ToString()
{
	LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
	// completed for you
	ostringstream temp;
	for (int loop =0; loop<5; loop++)
	{
		temp << "data[" << loop << "] value: " << data[loop] << endl;
	}

	LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "End Returning: " << temp.str()  << endl;
	return temp.str();
}

bool StaticArray::SetValue(string input ,int pos)
{
	LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "Start Params: " << input << "," << pos << endl;
	// store the value of parameter 1 into the array
	// at the position given in parameter 2
    bool temp = true;
    if (pos < 0 || pos > 4)
    {
        temp = false;
    }
    else
    {
        data[pos] = input;
    }
    LOG(INFO) << "End" << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "End Returning" << temp << endl;
    return temp;
}

