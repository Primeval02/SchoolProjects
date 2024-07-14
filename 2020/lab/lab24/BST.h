#pragma once

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




class BST
  {
  private:
    TreeNode *root; // The address of the root node
    TreeNode*	 insertSubtree(TreeNode *, ELEMTYPE);
    TreeNode *searchSubtree(TreeNode *, ELEMTYPE);
    void deallocate();
    void deallocateSubtree(TreeNode *);
    TreeNode *createNode(ELEMTYPE);

  public:
    BST();
    ~BST();
    bool empty();
    bool insert(ELEMTYPE);
    TreeNode *search(ELEMTYPE);
  };

















