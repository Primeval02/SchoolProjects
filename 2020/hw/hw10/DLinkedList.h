#pragma once
#include "cmpslib19.h"
#include "easylogging++.h"



template <class T>
class DLinkedList
{
	/* 
		 We need a node that is templated to store the same type as our 
		 Linked List. The easiest way to do this is make it an "Inner" class
		 It contains an element of the template type
		 and a pointer to the next node in the list.
	 */
	class DListNode
	{
		public:
			T data;
			DListNode *next;
            DListNode *prev;
			DListNode(T val) {data = val; next = nullptr;}
	};

    private:
    DListNode * Search(T val)
    {
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        DListNode *temp = head;
        DListNode * node;
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


	DListNode * head;
    DListNode * tail;

	public:
	DLinkedList()
	{
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
		// set head to the nullptr 
        head = nullptr;
        tail = nullptr;
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
	}

    ~DLinkedList()
    {
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        DListNode *temp = head;
        DListNode *temp2 = nullptr;
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
        DListNode *temp = new DListNode(val);
        temp->data = val;
        temp->next = head;
        temp->prev = nullptr;
        if (head != nullptr)
        {
            head->prev = temp;
        }
        head = temp;
        tail = Search(Back());
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
        return true;
	}

    bool InsertAfter(T val, T after)
    {
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        bool check;
        if(Search(after) != nullptr)
        {
            DListNode *prevNode = Search(after);
            DListNode *newNode = new DListNode(val);
            newNode->data = val;
            newNode->next = prevNode->next;
            prevNode->next = newNode;
            newNode->prev = prevNode;
            tail = Search(Back());
            if(newNode->next != nullptr)
            {
                newNode->next->prev = newNode;
            }
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
            DListNode *temp = head;
            DListNode *temp2 = head->next;
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
                            if (temp2->next != nullptr)
                            {
                                temp2->next->prev = temp;
                            }
                            delete temp2;
                            tail = Search(Back());
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
            DListNode * temp = head;
            return temp->data;
        }
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
    }

    T & Back()
    {
        LOG(INFO) << "Start " << __PRETTY_FUNCTION__ << endl;
        DListNode * temp3;
        if (empty() == true)
        {
            throw "List Empty";
        }
        else
        {
            DListNode * temp1 = head;
            DListNode * temp2 = head->next;
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
        DListNode *temp;
        temp = head;
        DListNode *temp2;
        temp2 = tail;
        ostringstream print;
        print << "DList: " << endl;
        print << "head: " << temp << endl;
        print << "tail: " << temp2 << endl;
        while(temp != nullptr)
        {
            print << "node:" << temp << " data:" << temp->data << " prev:" << temp->prev << " next:" << temp->next << endl;
            temp = temp->next;
        }
        LOG(INFO) << "End " << __PRETTY_FUNCTION__ << endl;
        LOG(INFO) << "Returning " << __PRETTY_FUNCTION__ << endl;
        return print.str();
	}
}; // end DLinkedList





