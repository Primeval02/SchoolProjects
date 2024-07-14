#pragma once
#include "cmpslib19.h"

#include <iostream>

// you can define ELEMTYPE prior to this to be another datatype
#ifndef ELEMTYPE
#define ELEMTYPE int
#endif

struct TreeNode
  {
  ELEMTYPE data;    // The element being stored
  TreeNode *left;   // The address of the left subtree
  TreeNode *right;  // The address of the right subtree
  };

class AVL
  {
  private:
    TreeNode *root; // The address of the root node
    TreeNode*	 insertSubtree(TreeNode *, ELEMTYPE);
    TreeNode *searchSubtree(TreeNode *, ELEMTYPE);
    void deallocate();
    void deallocateSubtree(TreeNode *);
    TreeNode *createNode(ELEMTYPE);

  public:
    AVL();
    ~AVL();
    bool empty();
    bool insert(ELEMTYPE);
    TreeNode *search(ELEMTYPE);
    TreeNode * iterativeSearch(ELEMTYPE);
  };


TreeNode *rightRotate(TreeNode *);
TreeNode *leftRotate(TreeNode *);
int height(TreeNode*);
















