#pragma once
#include "cmpslib19.h"
#include "easylogging++.h"

template <class T>
class Stack
{
	private:
		T * data;
		int TopIndex;
		int capacity;

// a private function that will double the capacity of the array and copy the values over ... resize
        void resize()
        {
            int newCapacity = capacity * 2;
            T * newData = new T[newCapacity];

            for (int loop = 0; loop < capacity - 1; loop++)
            {
                newData[loop] = data[loop];
            }

            capacity = newCapacity;
            data = newData;

        }

    public:
// you will need a constructor it will allow the user to pass in the size/capacity of the container as the only parameter
        Stack(int input = 4)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            TopIndex = 0;
            capacity = input;
            data = new T [capacity];
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }
// have this default to the value of 4



// destructor , clean up the array data
        ~Stack()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        }


// a public function that returns true or false if the container is empty
        bool empty()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            bool check;
            check = false;
            if (TopIndex == 0)
            {
                check = true;
            }
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
            return check;
        }


// a public function that returns true or false if the container is full
        bool full()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            bool check;
            check = false;
            if (TopIndex == capacity)
            {
                check = true;
            }
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
            return check;
        }

// a public function that allows a value to be added or  "pushed" into the stack

// if the container is full it calls resize then adds the value

// returns true
        bool Push(T input)
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            bool check;
            check = false;
            if (full() == true)
            {
                resize();
                data[TopIndex] = input;
                check = true;
            }

            else
            {
                data[TopIndex] = input;
                check = true;
            }
            TopIndex++;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
            return check;
        }


// a public function that will remove or "popp' a value from the stack

// true or false if pop operation was successful
        bool Pop()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            bool check;
            if (empty() == true)
            {
                check = false;
            }

            else 
            {
                check = true;
                TopIndex--;
            }
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
            return check;
        }


// a public function that returns the value at the top value on the stack

// throws a descriptive message if stack is empty
        T top()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            if (empty() == true)
            {
                throw "Cannot return top value if there are no values";
            }

            else
            {
                LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
                return data[TopIndex - 1];
            }
        }


// a public function that returns the capacity of the stack
        int Capacity()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
            return capacity;
        }


// a public function that returns the size of the stack
        int size()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
            return TopIndex;
        }


// ToString returns a string representation of the container.. see example for format
        string ToString()
        {
            LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
            ostringstream temp;
            temp << "ToString(): " << endl; 
            temp << "TopIndex: " << TopIndex << endl;
            temp << "Capacity(): " << Capacity() << endl;
            temp << "Empty(): " << boolalpha << empty() << endl;
            temp << "Full(): " << boolalpha << full() << endl;
            
            for (int loop = 0; loop < TopIndex; loop++)
            {
                temp << "data[" << loop << "]: address " << &data[loop] << " value " << data[loop] << endl;
            }

            LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
            return temp.str();
        }

}; // end Stack class


