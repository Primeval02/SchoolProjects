/*
	 All rights reserved. No part of this document may be reproduced, distributed,   or transmitted in any form or by any       means, including photocopying,             recording, or other electronic or mechanical methods, without the prior         written permission of the author, Michael Sarr
 */
#pragma once
#include "cmpslib19.h"
#include "fraction.h"
#include "easylogging++.h"


// NOTE THAT YOU CANNOT USE THE INDEX OPERATOR IN  BOTH SORTS
// AS DESCRIBED BELOW , READ IT

// SortAscending
// template function
//   use the index operator [] to access and modify the elements DO NOT use the Indirection  (*) operator
//   use the BUBBLE sort logic below and the logging suggested there as well
template <class T>
void SortAscending (T * array, int size)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    T temp;
    for (int outer = 0; outer < size; outer++)
    {
        for (int inner = 0; inner < size; inner++)
        {
            LOG(INFO) << "Comparing positions " << inner << " and " << inner + 1 << " their values are " << array[inner] << " and " << array[inner + 1] << __PRETTY_FUNCTION__ << endl;
            if (array[inner] > array[inner + 1])
            {
                LOG(INFO) << "Swapping positions " << inner << " and " << inner + 1 << __PRETTY_FUNCTION__ << endl;
                temp = array[inner];
                array[inner] = array[inner + 1];
                array[inner + 1] = temp;

            }
        }
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;

}



// SortDescending
// template function
//   use pointer arithmetic and the Indirection operator (*)   DO NOT use the  index operator ([])
//   use the BUBBLE sort logic below and the logging suggested there as well
template <class T>
void SortDescending (T * myArray, int size)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    T temp;
    for (int outer = 0; outer < size; outer++)
    {
        for (int inner = 0; inner < size; inner++)
        {
            LOG(INFO) << "Comparing positions " << inner << " and " << inner + 1 << " their values are " << myArray[inner] << " and " << myArray[inner + 1] << __PRETTY_FUNCTION__ << endl;
            if (*(myArray + inner) < *(myArray + inner + 1))
            {
                LOG(INFO) << "Swapping positions " << inner << " and " << inner + 1 << __PRETTY_FUNCTION__ << endl;
                temp = *(myArray + inner);
                *(myArray + inner) = *(myArray + inner + 1);
                *(myArray + inner + 1) = temp;
            }
        }
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;

}



// PrintArray
// template function
//   the format should be like:
//   Position:1 value:29
//   look at the output of the runnable example if you are unsure
template <class T>
void PrintArray (T * myArray, int size)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    for (int i = 0; i < size; i++)
    {
        cout << "Position:" << i << " Value:" << myArray[i] << endl; 
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;

}


// FillArrayWithRandom (pointer to the array of type Cfraction, size of the array)
// NOT A TEMPLATE function
//   fill the array pointed to by (target) with (count) elements
//   you MUST use the CreateRandomNumber function in cmpslib19.h
//   to change the value of a fraction you will need to call
//   SetNumerator and SetDenominator functions
//   make the numerator a random value from 0-100
//   make the denominatory random value from 1-100
void FillArrayWithRandom(CFraction * myArray, int count)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;   
    int numerator = 0;
    int denominator = 0;
    for (int i = 0; i < count; i++)
    {
        numerator = CreateRandomNumber(0, 100);
        denominator = CreateRandomNumber(1, 100);
        myArray[i].SetNumerator(numerator);
        myArray[i].SetDenominator(denominator); 
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;

}

// FillArrayWithRandom (pointer to the array, size of the array, min val, max val
// template function
//   fill the array pointed to by (target) with (count) elements
//   you MUST use the CreateRandomNumber function in cmpslib19.h
template <class T>
void FillArrayWithRandom(T * myArray, int count, int min, int max)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    T temp;
    for (int i = 0; i < count; i++)
    {
        temp = CreateRandomNumber(min, max);
        myArray[i] = temp; 
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}



// LargestInArray
// template function
// array parameter is "const T *"
//   return a copy of the largest value found
//   do not modify the values in the array
template <class T>
T LargestInArray(const T * array, int size)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    T largest = 0;
    for (int i = 0; i < size; i++)
    {
        if (array[i] > largest)
        {
            largest = array[i];
        }
    }
    return largest;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}



// SmallestInArray
// template function
// array parameter is "const T *"
//   return a copy of the smallest value found
//   do not modify the values in the array
template <class T>
T SmallestInArray(const T * array, int size)
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    T smallest = array[0];
    for (int i = 0; i < size; i++)
    {
        if (array[i] < smallest)
        {
            smallest = array[i];
        }
    }
    return smallest;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}






