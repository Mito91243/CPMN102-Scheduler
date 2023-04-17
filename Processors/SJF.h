#pragma once
#include"PriorityQueue.h"
#include"Processor.h"
class SJF :public Processor
{
private:
	PriorityQueue<process*>  sjfqueue;
public:
	void  addtoready(process* pr, int T)
	{
		sjfqueue.enqueue(pr, pr->getCT() - pr->getWON());
		pr->setstate('RDY');
	}
	
	process* Schedulealgo()
	{
		process* p;
		bool z=sjfqueue.dequeue(p);
		if (z)
			return p;
		else
			return nullptr;
	}
	process* getrun()
	{
		return running;
	}
	void printdata()
	{
		cout << sjfqueue.getCount() << "RDY:   ";
		sjfqueue.printpq();
	}
};

