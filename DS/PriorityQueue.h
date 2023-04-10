#pragma once
#include "PriorityNode.h"
#include"iostream"
template <typename T>
class PriorityQueue
{
private:
	PriorityNode<T>* frontPtr;
	int count;
public:
	int getCount() const
	{
		return count;
	}
	PriorityQueue()
	{
		frontPtr = nullptr;
		count = 0;
	}
	bool isEmpty() const
	{
		return !count;
	}
	bool enqueue(const T& newEntry, int pr)
	{
		PriorityNode<T>* newPtr = new PriorityNode<T>(newEntry, pr);

		if (isEmpty() || pr < frontPtr->getPriority())
		{
			newPtr->setNext(frontPtr);
			frontPtr = newPtr;
			count++;
			return true;
		}

		PriorityNode<T>* ptr = frontPtr;
		while (ptr->getNext() && ptr->getNext()->getPriority() <= pr)
			ptr = ptr->getNext();

		newPtr->setNext(ptr->getNext());
		ptr->setNext(newPtr);
		count++;
		return true;
	}
	bool dequeue(T& frntEntry)
	{
		if (isEmpty())
			return false;

		PriorityNode<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		frontPtr = frontPtr->getNext();
		// Queue is not empty; remove front

		// Free memory reserved by the dequeued node
		delete nodeToDeletePtr;

		count--;
		return true;

	}
	bool peekFront(T& frntEntry) const
	{
		if (isEmpty())
			return false;

		frntEntry = frontPtr->getItem();
		return true;

	}

	T* toArray()
	{
		if (!frontPtr)
			return nullptr;

		T* Arr = new T[count];
		PriorityNode<T>* p = frontPtr;
		for (int i = 0; i < count; i++)
		{
			Arr[i] = p->getItem();
			p = p->getNext();
		}
		return Arr;
	}
	
};
