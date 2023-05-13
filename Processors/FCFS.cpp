#include"Schudeler.h"
#include"FCFS.h"

FCFS::FCFS(int maxw, Schudeler* sch)
{
	MAXW = maxw;
	SCH = sch;
}
void FCFS::addtoready(process* pr)
{
	timeleft = timeleft + pr->getCT();
	RL.InsertEnd(pr);
	pr->setstate('RDY');
}

void FCFS::Schedulealgo(int T)
{
	process* pr = RL.DeleteFirst();
	//function and loop of migrate
	if (pr != nullptr) {
		if (!pr->getischild()) {
			if (T - pr->getAT() - pr->getWON() > MAXW) {
				bool f = SCH->migratetRR(pr);
				if (f && pr != nullptr) {
					while (T - pr->getAT() - pr->getWON() > MAXW) {
						timeleft = timeleft - pr->getCT();
						pr = RL.DeleteFirst();
						if (pr == nullptr) {
							running = nullptr;
							return;
						}
						if (T - pr->getAT() - pr->getWON() > MAXW) {
							f = SCH->migratetRR(pr);
						}
					}
				}
			}
		}
			running = pr;
			running->setstate('RUN');
	}
	else
		running = nullptr;
}
bool FCFS::pidcheck(process* pr, int p)
{
	return pr->getPID() == p;

}

process* FCFS::kill(int r, int t)
{
	process* p = nullptr;
	if (running)
	{
		if (*running == r)
		{



			p = running;
			Schedulealgo(t);
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

void FCFS::printdata()
{
	cout << RL.count() << " " << "RDY: ";
	RL.PrintList();
}

process* FCFS::getrun()
{
	return running;
}

process* FCFS::fork()
{
	if (running->getlchild() && running->getrchild())
	{
		return nullptr;
	}
	return running;
}

bool FCFS::orphan(process* p, int t)
{
	if (running == p)
	{
		running->setstate('ORPH');

		Schedulealgo(t);
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
