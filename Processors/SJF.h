#pragma once
#include"PiorityQueue.h"
#include"Processor.h"
class SJF :public Processor
{
private:
	PiorityQueue<process*>  sjfqueue;
	int busytime = 0;
	int processTRT = 0;

public:
	void addtoready(process* pr)
	{
		timeleft = timeleft + pr->getCT();
		sjfqueue.enqueue(pr, pr->getCT() - pr->getWON());
		pr->setstate('RDY');
	}

	process* Schedulealgo()
	{
		process* p = nullptr;
		bool z = sjfqueue.dequeue(p);
		
		if (z)
		{
			timeleft = timeleft - p->getCT();
			return p;
		}
		else
			return nullptr;
	}
	process* getrun()
	{
		return running;
	}
	void printdata()
	{
		std::cout << sjfqueue.getCount() << "RDY:   ";
		sjfqueue.printpq();
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
