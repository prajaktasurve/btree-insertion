#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  InternalNode* getParent();
  int getMinimum()const;
  InternalNode* insert(int value); // returns pointer to new InternalNode

  void insert(BTreeNode *node1, BTreeNode *node2); // if root splits use this
  InternalNode* insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);
  void resetMinimum();
  InternalNode* split(BTreeNode* newNode);
}; // InternalNode class

#endif
