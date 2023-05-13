#include"RRprocessor.h"
#include"Schudeler.h"
RRprocessor::RRprocessor(int ts, int rt, Schudeler* sch)
{
	timeslice = ts;
	RTF = rt;
	SCH = sch;
}
void RRprocessor::addtoready(process* pr)
{
	timeleft = timeleft + pr->getCT();
	Q1.enqueue(pr);
	pr->setstate('RDY');
}
void RRprocessor::Schedulealgo(int T)
{
	process* pr = nullptr;
	bool z = Q1.dequeue(pr);

	if (!z) {
		running = nullptr;
		return;
	}
	//function and loop of migrate
	if (pr->getCT() - pr->getWON() < RTF) {
		bool f = SCH->migratetsjf(pr);
		if (f && pr != nullptr) {
			while (pr->getCT() - pr->getWON() < RTF) {
				timeleft = timeleft - pr->getCT();
				z = Q1.dequeue(pr);
				if (!z) {
					running = nullptr;
					return;
				}
				if (pr->getCT() - pr->getWON() < RTF) {
					f = SCH->migratetsjf(pr);
				}
			}
		}
	}
	running = pr;
	TOP = 0;
	running->setstate('RUN');
}
process* RRprocessor::reachedts(int T)
{
	if (running)
	{
		TOP++;
	}

	if (TOP == timeslice) {
		process* finished = running;
		Schedulealgo(T);
		if (running != nullptr) {
			state = 'B';
			if (running->getRT() == 0)
			{
				running->setRT(T);
			}
		}
		else
		{
			state = 'I';
		}
		TOP = 0;
		return finished;
	}
	else return nullptr;
}

Queue<process*>RRprocessor::getqueue()
{
	return Q1;
}
void RRprocessor::printdata()
{
	std::cout << Q1.getCount() << " RDY: ";
	Q1.printq();
}
process* RRprocessor::getrun()
{
	return running;
}