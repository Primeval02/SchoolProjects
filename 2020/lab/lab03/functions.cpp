#include "functions.h"

// put your function bodies here
int TripleIntPBV(int value)
{
    LOG(INFO) << "Start: TripleIntPBV " << __PRETTY_FUNCTION__ << endl;
    int tripledValue = value * 3;
    LOG(INFO) << "End: TripleIntPBV " << __PRETTY_FUNCTION__ << endl;
    return tripledValue;
}

void TripleIntPBP(int * value)
{
    LOG(INFO) << "Start: TripleIntPBP " << __PRETTY_FUNCTION__ << endl;
    *value *= 3;
    LOG(INFO) << "End: TripleIntPBP " << __PRETTY_FUNCTION__ << endl;
}

void TripleIntPBR(int & value)
{
    LOG(INFO) << "Start: TripleIntPBR " << __PRETTY_FUNCTION__ << endl;
    value *= 3;
    LOG(INFO) << "End: TripleIntPBR " << __PRETTY_FUNCTION__ << endl;
}

void SwapIntPBP(int * value1, int * value2)
{
    LOG(INFO) << "Start: SwapIntPBP " << __PRETTY_FUNCTION__ << endl;
    int value3 = *value1;
    *value1 = *value2;
    *value2 = value3;
    LOG(INFO) << "End: SwapIntPBP " << __PRETTY_FUNCTION__ << endl;
}

void SwapIntPBR(int & value1, int & value2)
{
    LOG(INFO) << "Start: SwapIntPBR " << __PRETTY_FUNCTION__ << endl;
    int value3 = value1;
    value1 = value2;
    value2 = value3;
    LOG(INFO) << "End: SwapIntPBR " << __PRETTY_FUNCTION__ << endl;
}

int LargerInt(int value1, int value2)
{
    LOG(INFO) << "Start: LargerInt " << __PRETTY_FUNCTION__ << endl;
    int largerInt = (value1 >= value2) ? value1 : value2;
    LOG(INFO) << "End: LargerInt " << __PRETTY_FUNCTION__ << endl;
    return largerInt;
}

void LargerIntPBR(int value1, int value2, int & value3)
{
    LOG(INFO) << "Start: LargerIntPBR " << __PRETTY_FUNCTION__ << endl;
    value3 = (value1 >= value2) ? value1 : value2;
    LOG(INFO) << "End: LargerIntPBR " << __PRETTY_FUNCTION__ << endl;
}

void LargerIntPBP(int value1, int value2, int * value3)
{
    LOG(INFO) << "Start: LargerIntPBP " << __PRETTY_FUNCTION__ << endl;
    *value3 = (value1 >= value2) ? value1 : value2;
    LOG(INFO) << "End: LargerIntPBP " << __PRETTY_FUNCTION__ << endl;
}



