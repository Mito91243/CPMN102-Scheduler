#pragma once
#ifndef _FCFS
#define _FCFS

#include "LinkedList.h"
#include "Processor.h"

class FCFS : public Processor
{
private:
	process* running;
	LinkedList<process*> RL;

public:
	FCFS()
	{

	}
	void addtoready(process* pr,int timestep)
	{
		if (running == nullptr) {
			running = pr;
			running->setstate('RUN');
			if (running->getRT() == 0)
			{
				running->setRT(timestep);
			}
			return;
		}
		else
		{
		}
		RL.InsertEnd(pr);
		pr->setstate('RDY');


	}
	process* Schedulealgo()
	{
		process*p =  RL.DeleteFirst();
		if (p)
			return p;

			return nullptr;
	}
	process* randkill(int r)
	{
		int s = floor(r * (RL.count() + 1));
		process* p = RL.traverse(s);
		RL.DeleteNode(p);
		p->setstate('TRM');
		return p;
	}
	void printdata()
	{
		cout << RL.count() << " " << "RDY: ";
		RL.PrintList();
	}
	process* getrun()
	{
		return running;
	}
	process* changerun(int T)
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
};

#endif	
