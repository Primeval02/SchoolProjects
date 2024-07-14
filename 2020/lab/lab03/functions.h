#pragma once

#include "cmpslib19.h" // all the special functions provided for this class
#include "easylogging++.h"

//DO NOT use the same function name for multiple functions
//overloading a function name will be ambiguous so for this assignment DO NOT do it
//DO NOT use pass by refference or pass by pointer for ANY parameters that do not need to change
//the pass by value function is the only one that does not return void


//Create 3 functions that will triple an int value
//one function will be PBV, one function will be PBP and one function will be PBR
int TripleIntPBV(int value);

void TripleIntPBP(int * value);

void TripleIntPBR(int & value);

//create two functions that will allow a user to swap two integer values
//one function will have two PBP parameters
void SwapIntPBP(int * value1, int * value2);

void SwapIntPBR(int & value1, int & value2);

//create 3 functions that can be used to determine the larger of 2 integer values
//the other two functions will have 3 parameters, the result ( the larger of parameter one and two ) will be stored in the third
int LargerInt(int value1, int value2);

void LargerIntPBR(int value1, int value2, int & value3);

void LargerIntPBP(int value1, int value2, int * value3);
