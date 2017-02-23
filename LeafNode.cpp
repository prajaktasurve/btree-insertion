#include <iostream>
#include <algorithm>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()

InternalNode* LeafNode::getParent()
{
  return parent;
}

int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
  if (count < leafSize) // There is space
  {
    *(values + count) = value;
    sort(values, values + count + 1);
    count++;
    if (parent != NULL)
      parent->resetMinimum();
  }else
  {
      if (getLeftSibling() != NULL && getLeftSibling()->getCount() < leafSize) // Space in left sibling
      {
        if (value < getMinimum()) 
        {
          getLeftSibling()->insert(value);
        }else // Insert current minimum to left sibling
        {
          getLeftSibling()->insert(getMinimum());
          values[0] = value;
          sort(values, values + count);
        }
        if (parent != NULL)
          parent->resetMinimum();
      }else if (getRightSibling() != NULL && getRightSibling()->getCount() < leafSize) // Space in right sibling
      {
        if (values[count - 1] > value)
        {
          getRightSibling()->insert(values[count - 1]);
          values[count - 1] =  value;
          sort(values, values + count);
        }else
          getRightSibling()->insert(value);
        if (parent != NULL)
          parent->resetMinimum();
      }else
        return split(value);
  }
  return NULL; // to avoid warnings for now.
}  // LeafNode::insert()


void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()

LeafNode* LeafNode::split(int value)
{
  count = (count + 1)/2;
  LeafNode* newLeaf = new LeafNode (leafSize, parent, this, rightSibling);
  if (values[count - 1] < value) // move value to right leaf node
  {
    for(int k = count; k < leafSize; k++)
      newLeaf->insert(*(values + k));
    newLeaf->insert(value);
    
  }else{
    for(int k = count - 1; k < leafSize; k++)
      newLeaf->insert(*(values + k));
    values[count - 1] = value;
    sort(values, values + count + 1);
  }

  if (getRightSibling() != NULL)
    getRightSibling()->setLeftSibling(newLeaf);
  setRightSibling(newLeaf);
  if (parent != NULL)
    parent->resetMinimum();
  return newLeaf;
}

