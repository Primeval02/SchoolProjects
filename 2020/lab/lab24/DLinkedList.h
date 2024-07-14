#pragma once

#ifndef ELEMTYPE
#define ELEMTYPE int
#endif

// Define the list node structure. It contains the element and a pointer to
// the next and previous nodes in the list.
struct DListNode
  {
  ELEMTYPE data;
  DListNode *next;
  DListNode *prev;
  };


class DLinkedList
  {
  private:
    DListNode * head;
    DListNode * tail;
    DListNode * createNode(ELEMTYPE);
    bool InList(DListNode*);

  public:
    DLinkedList();
    ~DLinkedList();
    bool Insert(ELEMTYPE );
    bool InsertBack(ELEMTYPE);
    bool Empty();
    DListNode* Search(ELEMTYPE);
  };





