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


bool SortedBag::remove(TComp e) {
	int current = root;
	int prev = -1;
	while (current != -1)
	{
		if (elements[current].first == e)
		{
			if (elements[current].second > 1)
				elements[current].second--;
			else
			{
				if (current == root)
				{
					if (left[current] == -1 && right[current] == -1)
					{
						freeP(current);
						root = -1;
					}
					else if (left[current] != -1 && right[current] != -1)
					{
						int replacementNode = left[current];
						int prevReplacementNode = -1;
						while (right[replacementNode] != -1)
						{
							prevReplacementNode = replacementNode;
							replacementNode = right[replacementNode];
						}
						if (prevReplacementNode != -1)
							right[prevReplacementNode] = -1;
						if (replacementNode == left[current])
						{
							right[replacementNode] = right[current];
							root = replacementNode;
						}
						else
						{
							right[replacementNode] = right[current];
							left[replacementNode] = left[current];
							root = replacementNode;
						}
						freeP(current);
					}
					else
					{
						int rootToDelete = root;
						if (right[current] != -1)
							root = right[current];
						else
							root = left[current];
						freeP(rootToDelete);
					}
				}
				else
				{
					if (left[current] == -1 && right[current] == -1)
					{
						if (left[prev] == current)
							left[prev] = -1;
						else if (right[prev] == current)
							right[prev] = -1;
						freeP(current);
					}
					else if (left[current] != -1 && right[current] != -1)
					{
						int replacementNode = left[current];
						int prevReplacementNode = -1;
						while (right[replacementNode] != -1)
						{
							prevReplacementNode = replacementNode;
							replacementNode = right[replacementNode];
						}
						if (prevReplacementNode != -1)
							right[prevReplacementNode] = -1;
						if (replacementNode == left[current])
						{
							right[replacementNode] = right[current];
							if (left[prev] == current)
								left[prev] = replacementNode;
							else
								right[prev] = replacementNode;
						}
						else
						{
							right[replacementNode] = right[current];
							if (left[prev] == current)
								left[prev] = replacementNode;
							else
								right[prev] = replacementNode;
							left[replacementNode] = left[current];

						}
						freeP(current);
					}
					else
					{
						if (left[prev] == current)
						{
							if (left[current] != -1)
								left[prev] = left[current];
							else
								left[prev] = right[current];
						}
						else
						{
							if (left[current] != -1)
								right[prev] = left[current];
							else
								right[prev] = right[current];
						}
						freeP(current);
					}
				}
			}
			nrOfElements--;
			return true;
		}
		else if (relation(e, elements[current].first))
		{
			prev = current;
			current = left[current];
		}
		else
		{
			prev = current;
			current = right[current];
		}
	}
	return false;
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
		