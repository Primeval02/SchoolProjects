#pragma once
#include "cmpslib19.h"
#include "easylogging++.h"

/*
function1
name: Difference
pass by value 
returns the difference between the parmaters one and two (template type)
the difference should alwasy be positive (template type)
if the 2nd param is larger than the first throw "Invalid result"
like so 
if ( one > two)
		throw "Invalid Result";


while this is a template function it will only work with numeriic data types
test with double and int


function2
name: Difference
pass by reference
calculate the difference between the parmaters one   and two, store the result in parameter three
the difference should alwasy be positive (template type)
if the 2nd param is larger than the first throw "Invalid result"

while this is a template function it will only work with numeriic data types
test with double and int


function3
name: IsBetween
pass by value
is parameter one between parameters two (low) and three (high) the parameters are (template type)
if parameter two and three do not make a valid range throw "Invalid range"
return the result (bool)

function4
name: IsBetween
pass by reference
is parameter one between parameters two (low) and three (high) (template type)
if parameter two and three do not make a valid range throw "Invalid range"
store the result (bool) in parameter 4

test all 4 of your functions by calling them 4 times each , use two distinct data types , DO NOT call any function more than once with the exact same values
LOG THE START AND END OF ALL FUNCTIONS
 */

template <class T>
T Difference(T val1, T val2)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    T difference = val1 - val2;

    if (val2 > val1)
    {
        throw "Invalid Result";
    }

    return difference;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}

template <class T>
T Difference(T &val1, T &val2, T &val3)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    val3 = val1 - val2;

    if (val2 > val1)
    {
        throw "Invalid Result";
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}

template <class T>
T IsBetween(T val1, T val2, T val3)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    bool between;
    if (val1 > val2 && val1 < val3)
    {
        between = true;
    }
    else
    {
        between = false;
    }

    if (val2 >= val3)
    {
        throw "Invalid Range";
    }

    return between;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}

template <class T>
T IsBetween(T &val1, T &val2, T &val3, T &val4)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    if (val1 > val2 && val1 < val3)
    {
        val4 = true;
    }
    else
    {
        val4 = false;
    }

    if (val2 >= val3)
    {
        throw "Invalid Range";
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}
