#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"
#include "InternalNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;
  LeafNode* insert(int value); // returns pointer to new Leaf if splits

  void print(Queue <BTreeNode*> &queue);
  LeafNode* split (int value);
  InternalNode* getParent();
}; //LeafNode class

#endif
