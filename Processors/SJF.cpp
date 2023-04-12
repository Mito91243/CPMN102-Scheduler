#include "SJF.h"

SJF::SJF()
{
	count = 0;
	t = 0;
}



void SJF::addtoready(process* pr)
{
	sjfqueue.enqueue(pr, pr->getCT()-pr->getWON());

}


process* SJF::Schedulealgo()
{
	process* p;
		sjfqueue.dequeue(p);
		sjfqueue.peekFront(p);
		return p;
}

void SJF::printdata() {
	cout << sjfqueue.getCount() << "RDY";
	sjfqueue.printpq();
}
