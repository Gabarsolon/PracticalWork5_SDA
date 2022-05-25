#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>

using namespace std;

SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
	stack = new int[bag.capacity];
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
//Best case: Theta(1)
//Worst case: Theta(height)
//Total: O(height)

SortedBagIterator::~SortedBagIterator()
{
	delete[] stack;
}
//Theta(1)

TComp SortedBagIterator::getCurrent() {
	if (!valid())
		throw std::exception();
	return bag.elements[current].first;
}
//Theta(1)

bool SortedBagIterator::valid() {
	return current != -1;
}
//Theta(1)

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
//Best case: Theta(1)
//Worst case: Theta(height)
//Total: O(height)

void SortedBagIterator::first() {
	delete[] stack;
	stack = new int[bag.capacity];
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
//Best case: Theta(1)
//Worst case: Theta(height)
//Total: O(height)
