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

    private:
    SListNode * Search(T val)
    {
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        SListNode *temp = head;
        SListNode * node;
        if (temp == nullptr)
        {
            node = nullptr;
        }
        else
        {
            while (temp != nullptr)
            {
                if(temp->data == val)
                {
                    node = temp;
                    break;
                }
                else
                {
                    node = nullptr;
                    temp = temp->next;
                }
            }
        }
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
        return node;
    }


	SListNode * head;

	public:
	SLinkedList()
	{
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
		// set head to the nullptr 
        head = nullptr;
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
	}

    ~SLinkedList()
    {
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        SListNode *temp = head;
        SListNode *temp2 = nullptr;
        while(temp != nullptr)
        {
            temp2 = temp;
            temp = temp->next;
            delete temp2;
        }
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

    bool InsertAfter(T val, T after)
    {
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        bool check;
        SListNode *newNode = head;
        if(Search(after) != nullptr)
        {
            SListNode *temp = new SListNode(val);
            temp->next = newNode->next;
            newNode->next = temp;
            check = true;
        }
        else
        {
            check = false;
        }
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        LOG(INFO) << "Returning " << check << __PRETTY_FUNCTION__ << endl;
        return check;
    }

    bool empty()
    {
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        bool check;
        if(head == nullptr)
        {
            check = true;
        }
        else
        {
            check = false;
        }
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        LOG(INFO) << "Returning " << check << __PRETTY_FUNCTION__ << endl;
        return check;

    }

    bool Delete(T val)
    {
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        bool check;
        if (empty() == true)
        {
            check = false;
        }
        else
        {
            SListNode *temp = head;
            SListNode *temp2 = head->next;
            if(temp->data == val)
            {
                delete head;
                head = temp2;
                check = true;
            }
            else
            {
                if (temp2 == nullptr)
                {
                    check = false;
                }
                else
                {
                    while(temp2 != nullptr)
                    {
                        if (temp2->data == val)
                        {
                            temp->next = temp2->next;
                            delete temp2;
                            check = true;
                            break;
                        }
                        temp = temp->next;
                        temp2 = temp2->next;
                        check = false;
                    }
                }
            }
        }
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        LOG(INFO) << "Returning " << check << endl;
        return check;
    }

    bool Contains(T val)
    {
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        bool check;
        if (Search(val) != nullptr)
        {
            check = true;
        }
        else
        {
            check = false;
        }
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        LOG(INFO) << "Returning " << check << endl;
        return check;
    }

    T & Front()
    {
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        if (empty() == true)
        {
            throw "List Empty";
        }
        else
        {
            SListNode * temp = head;
            return temp->data;
        }
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    }

    T & Back()
    {
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        SListNode * temp3;
        if (empty() == true)
        {
            throw "List Empty";
        }
        else
        {
            SListNode * temp1 = head;
            SListNode * temp2 = head->next;
            while (temp1 != nullptr)
            {
                if (temp2 == nullptr)
                {
                    temp3 = temp1;
                    break;
                }
                else
                {
                    temp1 = temp1->next;
                    temp2 = temp2->next;
                }
            }
        }
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        return temp3->data;
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





