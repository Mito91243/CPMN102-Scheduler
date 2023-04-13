#include "SJF.h"

SJF::SJF()
{
	running = nullptr;
	count = 0;

}



void SJF::addtoready(process* pr,int T)

{
	if (running == nullptr)
	{
		running = pr;
		running->setstate('RUN');
		if (running->getRT() == 0)
		{
			running->setRT(T);
		}
		return;
	}
	else
	{
		sjfqueue.enqueue(pr, pr->getCT() - pr->getWON());
		running->setstate('RDY');
		count++;
	}
}


process* SJF::Schedulealgo()
{
	process* p;
		sjfqueue.dequeue(p);
		count--;
		return p;
}

process* SJF::getrun()
{
	return running;
}
void SJF::printdata() {
	cout << sjfqueue.getCount() << "RDY:   ";
	sjfqueue.printpq();
}


