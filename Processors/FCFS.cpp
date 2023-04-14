#pragma once
#include "FCFS.h"

void FCFS::addtoready(process* p, int t)
{
	if (running == nullptr)
	{
		running = p;
		p->setstate('RUN');
		if (p->getRT() == 0)
		{
			p->setRT(t);
		}
	}
	else
	{
		RL.InsertEnd(p);
		p->setstate('RDY');
	}
}

process* FCFS::Schedulealgo()
{
	return RL.DeleteFirst();
}

process* FCFS::randkill(int r)
{
	int s = floor(r * (RL.count() + 1));
	process* p = RL.traverse(s);
	RL.DeleteNode(p);
	p->setstate('TRM');
}

void FCFS :: printdata()
{
	cout << RL.count() << " " << "RDY: ";
	RL.PrintList();
}
