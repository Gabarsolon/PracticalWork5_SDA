#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>

using namespace std;

SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
	int node = b.root;
	stackIndex = 0;
	while (node != -1)
	{
		stack[stackIndex++] = node;
		node = b.left[node];
	}
	if (stackIndex != 0)
		current = stack[--stackIndex];
	else
		current = -1;
	currentOccurence = 1;
}

TComp SortedBagIterator::getCurrent() {
	if (!valid())
		throw std::exception();
	return bag.elements[current].first;
}

bool SortedBagIterator::valid() {
	return current != -1;
}

void SortedBagIterator::next() {
	if (!valid())
		throw std::exception();
	if (currentOccurence >= bag.elements[current].second)
	{
		int node = bag.right[current];
		while (node != -1)
		{
			stack[stackIndex++] = node;
			node = bag.left[node];
		}
		if (stackIndex != 0)
			current = stack[--stackIndex];
		else
			current = -1;
		currentOccurence = 0;
	}
	currentOccurence++;
}

void SortedBagIterator::first() {
	int node = bag.root;
	stackIndex = 0;
	while (node != -1)
	{
		stack[stackIndex++] = node;
		node = bag.left[node];
	}
	if (stackIndex != 0)
		current = stack[--stackIndex];
	else
		current = -1;
	currentOccurence = 1;
}

