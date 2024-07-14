#pragma once
#include "cmpslib19.h"
#include "easylogging++.h"


#ifndef QUEUESIZE
#define QUEUESIZE 10
#endif

#ifndef ELEMTYPE
#define ELEMTYPE int
#endif

class Queue
{
	private:
		ELEMTYPE data[QUEUESIZE];
		int backIndex;

	public:

		Queue();
		~Queue();
		bool empty();
		bool full();
		int size();
		bool enQueue(ELEMTYPE elem);
		bool deQueue();
		ELEMTYPE peekFront();
		string ToString();// return a string representation of the data in the container..

		string MACRO_VALUES();// return a string representation of the MACRO values

};

// the size is the difference between the back and front

// for this particular queue impementation the front index is always 0

int Queue::size()
{
	LOG(INFO) << "Start " <<  __PRETTY_FUNCTION__ << endl;

	LOG(INFO) << "End " <<  __PRETTY_FUNCTION__ << endl;
	LOG(INFO) << "Returning: " << backIndex << endl;
	return backIndex;
}



// Initialize the Queue

Queue::Queue( )
{
	// set the backIndex to 0
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    backIndex = 0;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}


Queue::~Queue()
{
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
}


bool Queue::empty()
{
	// return a bool value indicating if the Queue is empty
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    bool check;
    check = false;
    if (backIndex == 0)
    {
        check = true;
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
    return check;
}


// Check if the Queue is currently full

bool Queue::full()
{
	// return a bool value indicating if the Queue is full
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    bool check;
    check = false;
    if (backIndex == QUEUESIZE)
    {
        check = true;
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
    return check;
}


// Add an element to the top of the Queue

bool Queue::enQueue(ELEMTYPE elem)
{
	// if the Queue is full return false
// insert the value elem to the array at the position of backIndex
// increment the backIndex 
// return true
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    bool check;

    if (full() == true)
    {
        check = false;
    }

    else
    {
        data[backIndex] = elem;
        check = true;
        backIndex++;
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
    return check;


}


// Remove the element at the front of the Queue

bool Queue::deQueue()
{
	// if the Queue is empty return false
// shift all the elements in the array over one positon
// replace  element 0 ( front ) with the one at index 1
//  1 with 2 , 2 with 3, 3 with 4 and so on
// decrement the backIndex
// return true
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    bool check;
    if (empty() == true)
    {
        check = false;
    }

    else
    {
        for (int loop = 0; loop < backIndex - 1; loop++)
        {
            LOG(INFO) << "Copying value from index " << loop + 1 << " to index " << loop << __PRETTY_FUNCTION__ << endl;
            data[loop] = data[loop + 1];
        }
        check = true;
        backIndex--;
    }
    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
    return check;

}




// Retrieve the value at the top of the Queue but do not delete it

ELEMTYPE Queue::peekFront()
{
	// if the queue is empty throw a the message "cannot peekFront() on and empty queue"
// return the value at the top of the Queue
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    if (empty() == true)
    {
        throw "cannot peekFront() on an empty queue";
    }
    else
    {
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
        return data[0];
    }

}


string Queue::ToString()
{
	// create a string reresentation of the Queue
// add in the value from MACRO_VALUES()
// show the backIndex, the values returnted by the functions size, empty, full
// list the value of all the postitions of the array  ie
// data[0]:addres 0x000048 value:duck
// data[1]:addres 0x000052 value:chicken
    LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
    ostringstream temp;
    temp << MACRO_VALUES() << endl;
    temp << "backIndex: " << backIndex << endl;
    temp << "size(): " << size() << endl;
    temp << "empty(): " << empty() << endl;
    temp << "full(): " << full() << endl;

    for (int loop = 0; loop < backIndex; loop++)
    {
        temp << "data[" << loop << "]: address " << &data[loop] << " value: " << data[loop] << endl;
    }

    LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    return temp.str();
}


string Queue::MACRO_VALUES()
{
	ostringstream oss;
  ELEMTYPE x;
	oss << "ELEMTYPE: " << GetClassName(x)  << endl 
		<< "QUEUESIZE: "      << QUEUESIZE;

 return oss.str();
	}


