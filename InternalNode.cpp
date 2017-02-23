#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()

InternalNode* InternalNode::getParent()
{
  return parent;
}

int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value) // Doesn't handle INternAlNode Splitting.
{
  BTreeNode* tmp;
  int index = -1;
  for (int k = 0; k < count; k++)
    if (keys[k] < value)
      index = k;
  if (index > -1)
    tmp = children[index]->insert(value); // Inserted into childern
  else
    tmp = children[0]->insert(value); // Inserted into 1st child because it is smaller

  if (tmp != NULL)
    return insert(tmp);


  return NULL; // to avoid warnings for now.
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *node1, BTreeNode *node2)
{ 
  node1->setParent(this);
  node2->setParent(this);
  insert(node1);
  insert(node2);
} // InternalNode::insert()

InternalNode* InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  
  if (count < internalSize)
  {
    int index = -1;
    for (int k = 0; k < count; k++)
    {
      if (children[k]->getMinimum() < newNode->getMinimum())
        index = k;  
    }
    index++;

    for (int k = count; k > index; k--)
      children[k] = children[k-1];
    
    children[index] = newNode;
    newNode->setParent(this);
    if (index - 1 > -1)
      newNode->setLeftSibling(children[index - 1]);
    if (index + 1 < count)
      newNode->setRightSibling(children[index + 1]);
    count++;
    resetMinimum();
  }else{
    if (getLeftSibling() != NULL && getLeftSibling()->getCount() < internalSize) // Space in left sibling
      {
        if (newNode->getMinimum() < getMinimum()) 
        {
          ((InternalNode*)getLeftSibling())->insert(newNode);
        }else // Insert current minimum to left sibling
        {
          ((InternalNode*)getLeftSibling())->insert(children[0]);
          count--;
          for (int k = 0; k < count; k++)
            children[k] = children[k + 1];
          insert(newNode);
          if (parent != NULL)
            parent->resetMinimum(); 
        }

      }else if (getRightSibling() != NULL && getRightSibling()->getCount() < internalSize) // Space in right sibling
      {
        if (keys[count - 1] > newNode->getMinimum())
        {
          ((InternalNode*)getRightSibling())->insert(children[count-1]);
          count--; 
          insert(newNode);
        }else
          ((InternalNode*)getRightSibling())->insert(newNode);
        
        if(parent != NULL)
          parent->resetMinimum();
      }else
      {
        
          return split(newNode); //root split
      }   
  }

  return NULL;
} // InternalNode::insert()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()

void InternalNode::resetMinimum()
{
  for(int k = 0; k < count; k++)
    keys[k] = children[k]->getMinimum();
  if(parent)
    parent->resetMinimum();
}

InternalNode* InternalNode::split (BTreeNode* newNode) // <-- leafnode
{
  count = (count + 1)/2;
  InternalNode* newInt = new InternalNode (internalSize, leafSize, NULL, NULL, NULL);
  if (keys[count - 1] < newNode->getMinimum()) // move new node to right leaf node
  {
    for(int k = count; k < internalSize; k++)
      newInt->insert(children[k]);
    newInt->insert(newNode);
  
  }else{
    for(int k = count -1; k < internalSize; k++)
      newInt->insert(children[k]);
    count--;
    insert(newNode);
  }

  resetMinimum();
  return newInt;
}