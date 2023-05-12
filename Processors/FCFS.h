#pragma once
#ifndef _FCFS
#define _FCFS

#include "LinkedList.h"
#include "Processor.h"

class FCFS : public Processor
{
private:
	LinkedList<process*> RL;
	int busytime;
	int processTRT;

public:

	void addtoready(process* pr)
	{
		timeleft = timeleft + pr->getCT();
		RL.InsertEnd(pr);
		pr->setstate('RDY');
	}

	process* Schedulealgo()
	{
		process* p = RL.DeleteFirst();
		
		if(p)
		{
			timeleft = timeleft - p->getCT();
			return p;
		}

		return nullptr;
	}

	static bool pidcheck(process* pr, int p)
	{
		return pr->getPID() == p;

	}

	process* kill(int r, int t)
	{
		process* p = nullptr;
		if (running)
		{
			if (*running == r)
			{
				p = running;
				running = Schedulealgo();
				if (running)
				{
					running->setstate('RUN');
					running->setRT(t);
				}
			}
			else if (RL.count() != 0)
			{
				p = RL.Find(r);


				RL.DeleteNode(p);
				
			}
			if (p)
				p->setstate('TRM');
		}
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

	process* fork()
	{
		if (running->getlchild() && running->getrchild())
		{
			return nullptr;
		}
		return running;
	}

	bool orphan(process* p, int t)
	{
		if (running == p)
		{
			running->setstate('ORPH');
			running = Schedulealgo();
			if (running)
			{
				running->setstate('RUN');
				running->setRT(t);
				state = 'B';
			}
			else
				state = 'I';
			return true;
		}
		if (RL.DeleteNode(p))
		{		
			timeleft = timeleft + p->getCT();
			p->setstate('ORPH');
			return true;
		}
		return false;
	}

	int getIDLE(int& TotalTRT)
	{
		if (running != NULL)
		{
			busytime++;
			processTRT = processTRT + running->getTRT();

		}

		TotalTRT = processTRT;
		return busytime;
	}
};

#endif	
