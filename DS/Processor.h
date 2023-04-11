#pragma once
#include"process.cpp"
class Processor
{
protected:
	process* running; // pointer to what the process is working on
public:
	virtual process* changerun(int T) {
		if (running != nullptr)
		{
			process* temp = running;
			running = Schedulealgo();
			if (running->getRT() == 0) {
				running->setRT(T);
			}
			return temp;
		}
		return nullptr;
	}
	virtual process* isfinished(int T)
	{
		if (running != nullptr)
		{
			running->incrementWON();
			process* temp = running;
			if (running->getWON() == running->getCT())
			{
				running = Schedulealgo();
				running->setstate('RUN');
				if (running->getRT() == 0) {
					running->setRT(T);
				}
				temp->setstate('TRM');
				return temp;

			}
		}
		return nullptr;
	}
	// increments WON and checks if it is equal to its CT,and if yes changes the running pointer to the its next process sets its state to R and sets its RT  
	//and returns a pointer to the finished one to be added to the termminated
	//if no returns a nullptr
	virtual void addtoready(process* pr,int T) = 0;//adds to the list
	virtual process* Schedulealgo() = 0;
	virtual process* needsIO(int T)
	{
		process* temp = running;
		if (running->getWON() == running->getIO_R())
		{
			running = Schedulealgo();
			if (running->getRT() == 0) {
				running->setRT(T);
			}
			temp->setstate('BLK');
			return temp;
		}
		else return nullptr;
	}
	//checks if WON is equal to its next IO_R,and if yes changes the running pointer to the its next process sets its state to R and sets its RT
	//and returns a pointer to the one that needs IO to be added to the blocked
	//if no returns a nullptr
};
