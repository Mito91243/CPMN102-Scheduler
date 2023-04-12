#include "RRprocessor.h"
RRprocessor::RRprocessor(int ts) {
	timeslice = ts;
	running = nullptr;
}
void RRprocessor:: addtoready(process* pr,int T) {
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
	else {
		Q1.enqueue(pr);
		running->setstate('RDY');
	}
}
process* RRprocessor::Schedulealgo() {
	process* pr;
	bool z = Q1.dequeue(pr);
	TOP = 0;
	if (z)
		return pr;
	else
		return nullptr;
}
/*process* RRprocessor::isfinished(int T) {
	running->incrementWON();
	if (running->getWON() == running->getCT()) {
		process* finished = running;
		process* pr;
		bool z = Q1.dequeue(pr);
		if (!z) {
			return finished;
		}
		else {
			running = pr;
			running->setstate('r');
			if (running->getRT() == 0) {
				running->setRT(T);
			}
			return finished;
		}

	}
	else return nullptr;
}*/

/*process* RRprocessor::needsIO(int T) {
	if (running->getIO() == running->getNIO()) {
		return nullptr;
	}
	if (running->getWON() == running->getIO_R()) {
		process* finished = running;
		process* pr;
		bool z = Q1.dequeue(pr);
		if (!z) {
			return finished;
		}
		else {
			running = pr;
			running->setstate('r');
			if (running->getRT() == 0) {
				running->setRT(T);
			}
			return finished;
		}

	}
	else return nullptr;
}*/
process* RRprocessor::reachedts(int T) {
	if (running != nullptr) {
		running->incrementWON();
	}
	TOP++;
	if (TOP == timeslice) {
		process* finished = running;
		running = Schedulealgo();
		if (running != nullptr) {
			running->setstate('RUN');
			if (running->getRT() == 0) {
				running->setRT(T);
			}
		}
		return finished;
	}
	else return nullptr;

}
Queue<process*>RRprocessor::getqueue() {
	return Q1;
}
void RRprocessor::printdata() {
	cout << Q1.getCount() << "RDY";
	Q1.printq();
}
