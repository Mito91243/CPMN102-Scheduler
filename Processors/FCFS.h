#pragma once
#ifndef _FCFS
#define _FCFS

#include "LinkedList.h"
#include "Processor.h"

class FCFS : public Processor
{
private:
	LinkedList<process*> RL;
public:
	void addtoready(process* pr,int timestep)
	{
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
	process* randkill(double r)
	{
		if (RL.count() != 0)
		{
			int s = floor(r * (RL.count()));
			process* p = RL.traverse(s);
			RL.DeleteNode(p);
			p->setstate('TRM');
			return p;
		}
		return nullptr;
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

#endif	
