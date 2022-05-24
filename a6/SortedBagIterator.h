#pragma once
#include "SortedBag.h"

class SortedBag;

class SortedBagIterator
{
	friend class SortedBag;

private:
	const SortedBag& bag;
	SortedBagIterator(const SortedBag& b);
	int stack[1000];
	int stackIndex;
	int current;
	int currentOccurence;

public:
	TComp getCurrent();
	bool valid();
	void next();
	void first();
};

