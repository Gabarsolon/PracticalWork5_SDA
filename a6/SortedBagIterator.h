#pragma once
#include "SortedBag.h"

class SortedBag;

class SortedBagIterator
{
	friend class SortedBag;

private:
	const SortedBag& bag;
	SortedBagIterator(const SortedBag& b);
	int* stack;
	int stackIndex;
	int current;
	int currentOccurence;

public:
	~SortedBagIterator();
	TComp getCurrent();
	bool valid();
	void next();
	void first();
};

