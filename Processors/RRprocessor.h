#pragma once
#include"Processor.h"
#include"process.h"
#include"Queue.h"

class RRprocessor : public Processor
{
private:
	int timeslice;
	Queue<process*> Q1;
	int TOP;

public:
	
	RRprocessor(int ts)
	{
		timeslice = ts;
	}
	virtual void addtoready(process* pr)
	{
			Q1.enqueue(pr);
			pr->setstate('RDY');
	}
	virtual process* Schedulealgo()
	{
		process* pr;
		bool z = Q1.dequeue(pr);
		if (z)
			return pr;
		else
			return nullptr;
	}
	process* reachedts(int T)
	{
		if(running)
		{ 
			TOP++;
		}
		if (TOP == timeslice) {
			process* finished = running;
			process* pr;
			bool z = Q1.dequeue(pr);
			if (!z) {
				state = 'I';
				running = nullptr;
				return finished;
			}
			else {
				state = 'B';
				running = pr;
				running->setstate('RUN');
				if (running->getRT() == 0) {
					running->setRT(T);
				}
				TOP = 0;
				return finished;
			}

		}
		else return nullptr;
	}

	Queue<process*> getqueue()
	{
		return Q1;
	}
	void printdata()
	{
		cout << Q1.getCount() << " RDY: ";
		Q1.printq();
	}
	process* getrun()
	{
		return running;
	}

	int  getWT()
	{
		return Q1.sum();
	}
	
};
