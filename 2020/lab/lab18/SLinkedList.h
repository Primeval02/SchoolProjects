#pragma once
#include "cmpslib19.h"
#include "easylogging++.h"



template <class T>
class SLinkedList
{
	/* 
		 We need a node that is templated to store the same type as our 
		 Linked List. The easiest way to do this is make it an "Inner" class
		 It contains an element of the template type
		 and a pointer to the next node in the list.
	 */
	class SListNode
	{
		public:
			T data;
			SListNode *next;
			SListNode(T val) {data = val; next = nullptr;}
	};

	SListNode * head;

	public:
	SLinkedList()
	{
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
		// set head to the nullptr 
        head = nullptr;
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
	}


	bool Insert(T val)
	{
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
		// insert a new node to store val 
// at the head of the list
// create a new node to store the value to be added
// set the next pointer of the new node to point to the current first node in the list
// make this node the new front by setting the head pointer to it
        SListNode *temp = new SListNode(val);
        temp->data = val;
        temp->next = head;
        head = temp;
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
        return true;
	}

	string ToString()
	{
		// return a text representation of the stack
// IT MUST WORK WITH THE WEBPAGE
// make sure to test your output
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        SListNode *temp;
        temp = head;
        ostringstream print;
        print << "SList: " << endl;
        print << "head: " << temp << endl;
        while(temp != nullptr)
        {
            print << "node:" << temp << " data:" << temp->data << " next:" << temp->next << endl;
            temp = temp->next;
        }

        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
        return print.str();
	}
}; // end SLinkedList





