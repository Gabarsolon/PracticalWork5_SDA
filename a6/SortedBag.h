#pragma once
//DO NOT INCLUDE SORTEDBAGITERATOR
#include <utility>
//DO NOT CHANGE THIS PART
typedef int TComp;
typedef TComp TElem;
typedef bool(*Relation)(TComp, TComp);
typedef std::pair<TComp, int> TPair;

#define NULL_TCOMP -11111;
#define NULL_TPAIR pair<TComp, int>(-111111, -111111);


class SortedBagIterator;

class SortedBag {
	friend class SortedBagIterator;

private:
	static const int capacity = 1000;
	int nrOfElements;
	int firstFree;
	int root;
	TPair elements[capacity];
	int right[capacity];
	int left[capacity];
	Relation relation;

public:
	int allocateP();
	void freeP(int i);
	//constructor
	SortedBag(Relation r);

	//adds an element to the sorted bag
	void add(TComp e);

	//removes one occurence of an element from a sorted bag
	//returns true if an eleent was removed, false otherwise (if e was not part of the sorted bag)
	bool remove(TComp e);

	//checks if an element appearch is the sorted bag
	bool search(TComp e) const;

	//returns the number of occurrences for an element in the sorted bag
	int nrOccurrences(TComp e) const;

	//returns the number of elements from the sorted bag
	int size() const;

	//returns an iterator for this sorted bag
	SortedBagIterator iterator() const;

	//checks if the sorted bag is empty
	bool isEmpty() const;

	//destructor
	~SortedBag();
};