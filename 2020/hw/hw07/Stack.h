#pragma once
#include "cmpslib19.h"
#include "easylogging++.h"
#include "fraction.h"
#include <array>

#ifndef ELEMTYPE  // only do the following define if its not already defined

#define ELEMTYPE  int
#endif

// create your stack class, it will store values of type ELEMTYPE 
class stack
{
    private:
        int current;
        int capacity;
        ELEMTYPE * mystack;

    public:
// you will need a constructor it will allow the user to pass in the size/capacity of the container as the only parameter
// have this default to the value of 4
        stack(int size = 4)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            current = 0;
            capacity = size;
            mystack = new ELEMTYPE[capacity];
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }
// a public function that returns true or false if the container is empty
        bool empty()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            if (mystack.empty() == true)
            {
                LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
                return true;
            }
            else
            {
                LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
                return false;
            }

        }
// a public function that returns true or false if the container is full
        bool full()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            for(int loop = 0; loop < capacity; loop++)
            {
                if (mystack[loop] != NULL)
                {
                    count++;
                }
            }
            
            if (count == 4)
            {
                LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
                return true;
            }
            else
            {
                LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
                return false;
            }
            
        }
// a public function that returns the capacity of the stack
        int cap()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            int size;
            size = sizeof(mystack);
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return size;
           
        }
// a function that returns the number of elements currently in the stack
        int size()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            int size;
            size = sizeof(mystack)/sizeof(mystack[0]);
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return size;
        }
// a public function that allows a value to be added or  "pushed" into the stack, returns 
// true or false if push operation was successful
        bool push(ELEMTYPE input)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            mystack[current] = input;
            if (mystack[current] == input)
            {
                current++;
                return true;
                LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            }
            current++;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return false;
        }
// a public function that will remove or "pop' a value from the stack
// true or false if pop operation was successful
        bool pop()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            mystack[current] = NULL;
            if (mystack[current] == NULL)
            {
                current--;
                return true;
                LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            }
            current--;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return false;
        }
// a public function that returns a reference to the top value on the stack
// if the stack is empty return the value at index 0
        ELEMTYPE top()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;   
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return mystack[current];
        }
// blankline
// a public ToString() function that matches the examples output
};


