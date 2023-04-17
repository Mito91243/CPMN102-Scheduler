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
	virtual void addtoready(process* pr, int T)
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
		TOP++;
		if (TOP == timeslice) {
			process* finished = running;
			process* pr;
			bool z = Q1.dequeue(pr);
			if (!z) {
				return finished;
			}
			else {
				running = pr;
				running->setstate('RUN');
				if (running->getRT() == 0) {
					running->setRT(T);
				}
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

	process* isfinished(int T) {
		running->incrementWON();
		if (running->getWON() == running->getCT()) {
			process* finished = running;
			process* pr;
			bool z = Q1.dequeue(pr);
			if (!z) {
				return finished;
			}
			else {
				running = pr;
				running->setstate('RUN');
				if (running->getRT() == 0) {
					running->setRT(T);
				}
				return finished;
			}
		}
		else return nullptr;
	}
	process* getrun()
	{
		return running;
	}
	
	/*process* changerun(int T)
	{
		process* temp = running;

		if (running != nullptr)
		{
			running = Schedulealgo();
			if (running != nullptr) {
				running->setstate('RUN');
				if (running->getRT() == 0) {
					running->setRT(T);
				}
			}
		}
		return temp;
	}
	*/
};
