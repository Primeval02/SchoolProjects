#pragma once

#ifndef ELEMTYPE
#define ELEMTYPE int
#endif

struct SListNode
  {
  ELEMTYPE data;
  SListNode *next;
  };


class SLinkedList
  {
  private:
    SListNode * head;
    SListNode * createNode(ELEMTYPE);
    bool InList(SListNode*);

  public:
    SLinkedList();
    ~SLinkedList();
    bool Insert(ELEMTYPE );
    bool InsertBack(ELEMTYPE);
    bool Empty();
    SListNode* Search(ELEMTYPE);
  };





