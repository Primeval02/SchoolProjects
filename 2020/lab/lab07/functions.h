#pragma once
#include "cmpslib19.h"
#include "easylogging++.h"



int Sort1(int *array, int N)
{
	// a basic bubble sort 
// for loop outer ( 0 to N-1)
// for loop inner ( 0 to N-2)
// compare two adjacent items (inner and inner+1) and std::swap if necessary
//    
// use an int to keep track of how many comparisons occur and return it  
// 
// basically requires N squared iterations, always the same 
// log the start , end and the value returned ( look at the example log)
LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
int comparisons = 0;
for (int outer = 0; outer < (N - 1); outer++)  // this should probaby be N
{
    for(int inner = 0; inner < (N); inner++) // this looks right
    {
        if (array[inner] > array[inner + 1])
        {  
            std::swap(array[inner], array[inner + 1]);
        }
        comparisons++;
    }
}
LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
LOG(INFO) << "Returning " << comparisons << endl;
return comparisons;
}


int Sort2(int *array, int N)
{
	// for loop outer( 0 to N-1 )
// for loop inner( 0 to (N-1 - outer) )     
// compare two adjacent items (inner and inner+1) and std::swap if necessary
//
// use an int to keep track of how many comparisons occur and return it  
// basically requires (n(n+1))/2 iterations, always the same
// log the start , end and the value returned ( look at the example log)
LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
int comparisons = 0;
for(int outer = 0; outer < (N - 1); outer++)
{
    for(int inner = 0; inner < (N - 1 - outer); inner++)
    {
        if (array[inner] > array[inner + 1])
        {
            std::swap(array[inner], array[inner + 1]);
        }
        comparisons++;
    }
}
LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
LOG(INFO) << "Returning " << comparisons << endl;
return comparisons;
} 


int Sort3(int *array, int N)
{
	// same basic logic as Sort2
  // except the outer loop will not be a for..loop  it will now be a do..while loop so we can stop as soon as it is sorted
  // how do we know if its sorted .. if we traverse the array all the way from front to back  and we dont
  // need to swap any it must be sorted

	// there is a bool flag "IsSorted" set to false
	// we set it true before each pass through the array (inner loop)  
	// if during the pass through the array we need to swap two values it is not sorted
	// so set storted to false so we will repeat the do.. while loop one more time at least
	//
	// use an int to keep track of how many comparisons occur and return it  
	// basically <= (n(n+1) / 2  iterations, can vary greatly depending on how unsorted the target is
  // log the start , end and the value returned ( look at the example log)

	LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;
	bool sorted=false;
	int outer=0;
	int comparisons=0;

  do 
	{
		sorted=true;
    // copy your inner loop and sort from Sort2
    // if you do have to swap a value then also set sorted=false;
//blankline

        for(int inner = 0; inner < (N - 1 - outer); inner++)
        {
            if (array[inner] > array[inner + 1])
            {
                sorted = false;
                std::swap(array[inner], array[inner + 1]);
            }
            comparisons++;
           
        }

	  // after the inner loop and traversing the array
// sorted == true if we didnt need to swap 
// sorted == false if we did need to swap  

		outer++;
	}
	while( ! sorted ); // if sorted is false repeat the process

	LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "Returning " << comparisons << endl;
	return comparisons;


}
