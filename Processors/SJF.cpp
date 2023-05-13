#include"SJF.h"
SJF::SJF(Schudeler* sch)
{
	SCH = sch;
}
void SJF::addtoready(process* pr)
{
	timeleft = timeleft + pr->getCT();
	sjfqueue.enqueue(pr, pr->getCT() - pr->getWON());
	pr->setstate('RDY');
}

void SJF::Schedulealgo(int T)
{
	process* pr = nullptr;
	bool z = sjfqueue.dequeue(pr);

	if (z)
	{
		running = pr;
		running->setstate('RUN');
		timeleft = timeleft - pr->getCT();
	}
	else
		running=nullptr;
}
process* SJF::getrun()
{
	return running;
}
void SJF::printdata()
{
	std::cout << sjfqueue.getCount() << "RDY:   ";
	sjfqueue.printpq();
}
