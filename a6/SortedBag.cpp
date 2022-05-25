#include "SortedBag.h"
#include "SortedBagIterator.h"
#include <iostream>

void SortedBag::resize()
{
	capacity *= 2;
	int* newLeft = new int[capacity];
	int* newRight = new int[capacity];
	TPair* newElements = new TPair[capacity];
	for (int i = 0; i < capacity / 2; i++)
	{
		newLeft[i] = left[i];
		newRight[i] = right[i];
		newElements[i] = elements[i];
	}
	for (int i = capacity / 2; i < capacity - 1; i++)
	{
		newLeft[i] = i + 1;
		newRight[i] = i + 1;
	}
	newLeft[capacity - 1] = -1;
	newRight[capacity - 1] = -1;

	delete[] left;
	delete[] right;
	delete[] elements;

	left = newLeft;
	right = newRight;
	elements = newElements;

	firstFree = capacity/2;
}

int SortedBag::allocateP()
{
	if (firstFree == -1)
		return -1;
	int newFreePos = firstFree;
	firstFree = right[firstFree];
	return newFreePos;
}
//Theta(1)

void SortedBag::freeP(int i)
{
	right[i] = firstFree;
	left[i] = firstFree;
	firstFree = i;
}
//Theta(1)

SortedBag::SortedBag(Relation r) {
	capacity = 32;
	left = new int[capacity];
	right = new int[capacity];
	elements = new TPair[capacity];
	relation = r;
	nrOfElements = 0;
	root = -1;
	for (int i = 0; i < capacity - 1; i++)
	{
		right[i] = i + 1;
		left[i] = i + 1;
	}
	left[capacity - 1] = -1;
	right[capacity - 1] = -1;
	firstFree = 0;
}

void SortedBag::add(TComp e) {
	if (root == -1)
	{
		int pos = allocateP();
		elements[pos] = TPair(e, 1);
		left[pos] = -1;
		right[pos] = -1;
		root = pos;
		nrOfElements++;
	}
	else
	{
		if (firstFree == -1)
			resize();
		int current = root;
		int previous = -1;
		while (current != -1)
		{
			if (elements[current].first == e)
			{
				elements[current].second++;
				nrOfElements++;
				return;
			}
			if (this->relation(e, elements[current].first))
			{
				previous = current;
				current = left[current];
			}
			else
			{
				previous = current;
				current = right[current];
			}
		}
		int pos = allocateP();
		elements[pos] = TPair(e, 1);
		if (relation(e, elements[previous].first))
			left[previous] = pos;
		else
			right[previous] = pos;
		left[pos] = -1;
		right[pos] = -1;
		nrOfElements++;
	}
}


bool SortedBag::remove(TComp e)
{
	if (nrOfElements == 0)
		return false;
	if (nrOfElements == 1 && elements[root].first == e)
	{
		freeP(root);
		root = -1;
		nrOfElements--;
		return true;
	}
	if (nrOfElements == 1 && elements[root].first != e)
		return false;

	int currentPos = root;
	int prevPos = -1;
	while (currentPos != -1)
	{
		if (elements[currentPos].first == e)
			break;
		else if (relation(elements[currentPos].first, e) == true)
		{
			prevPos = currentPos;
			currentPos = right[currentPos];
		}
		else if (relation(elements[currentPos].first, e) == false)
		{
			prevPos = currentPos;
			currentPos = left[currentPos];
		}
	} //find if the key exists
	if (currentPos == -1)
		return false; //key does not exists
	else //key exists
	{
		if (elements[currentPos].second > 1) //if there is more than 1 value
		{
			elements[currentPos].second--;
			nrOfElements--;
			return true;
		}
		else
		{
				if ((left[currentPos] == -1) && (right[currentPos] == -1)) //leaf
				{
					removeLeaf(currentPos, prevPos);
					freeP(currentPos);
					nrOfElements--;
					return true;
				}
				if ((left[currentPos] == -1) || (right[currentPos] == -1)) //only 1 side descendants
				{
					removeNodeWithOneDescendant(currentPos, prevPos);
					freeP(currentPos);
					nrOfElements--;
					return true;
				}
				if ((left[currentPos] != -1) && (right[currentPos] != -1))
				{
					int nodeToReplace = currentPos;
					while (left[currentPos] != -1) //find the smallest node on the right subtree
					{
						prevPos = currentPos;
						currentPos = left[currentPos];
					}
					elements[nodeToReplace] = elements[currentPos];
					if ((left[currentPos] == -1) && (right[currentPos] == -1)) //leaf
					{
						removeLeaf(currentPos, prevPos);
						freeP(currentPos);
						nrOfElements--;
						return true;
					}
					else
					{
						removeNodeWithOneDescendant(currentPos, prevPos);
						freeP(currentPos);
						right[currentPos] = -1;
						nrOfElements--;
						return true;
					}

				}
		}
	}
	//key does not exist
	return false;
}

void SortedBag::removeLeaf(int leafPos, int parentPos) //lefts and rights of deleted node do not change
{
	if (left[parentPos] == leafPos) //leaf is left child
	{
		left[parentPos] = -1;
	}
	else
	{
		right[parentPos] = -1;
	}
}

void SortedBag::removeNodeWithOneDescendant(int nodePos, int parentPos) //lefts and rights of deleted node do not change
{
	if (parentPos != -1)
	{
		if (left[parentPos] = nodePos) //node is left child
		{
			if (right[nodePos] == -1) //node has only left descendants
			{
				left[parentPos] = left[nodePos];
			}
			else if (left[nodePos] == -1) //node has only right descendants
			{
				left[parentPos] = right[nodePos];
			}
		}
		else  //node is right child
		{
			if (right[nodePos] == -1) //node has only left descendants
			{
				right[parentPos] = left[nodePos];
			}
			else if (left[nodePos] == -1) //node has only right descendants
			{
				right[parentPos] = right[nodePos];
			}
		}
	}
	else //remove root
	{
		if (left[nodePos] == -1) //root has only right descendants
		{
			root = right[nodePos];
		}
		else if (right[nodePos] == -1) //root has only right descendants
		{
			root = left[nodePos];
		}
	}
}

bool SortedBag::search(TComp elem) const {
	int current = root;
	while (current != -1)
	{
		if (elements[current].first == elem)
			return true;
		else if (relation(elem, elements[current].first))
			current = left[current];
		else
			current = right[current];
	}
	return false;
}


int SortedBag::nrOccurrences(TComp elem) const {
	int current = root;
	while (current != -1)
	{
		if (elements[current].first == elem)
			return elements[current].second;
		else if (relation(elem, elements[current].first))
			current = left[current];
		else
			current = right[current];
	}
	return 0;
}



int SortedBag::size() const {
	return nrOfElements;
}


bool SortedBag::isEmpty() const {
	return nrOfElements == 0;
}


SortedBagIterator SortedBag::iterator() const {
	return SortedBagIterator(*this);
}


SortedBag::~SortedBag() {
	delete[] elements;
	delete[] left;
	delete[] right;
}
		