#pragma once
#include"process.h"

class Schudeler; //forward declaration

class Processor
{
protected:
	process* running = nullptr; // pointer to what the process is working on
	char state = 'I'; //I for IDLE, B for Busy,S 
	int n = 5;//overheat time
	int OT;//time spent overheating
	int timeleft = 0;
	int busytime = 0;
	Schudeler* SCH = nullptr;

public:

	process* changerun(int T)
	{
		process* temp = running;

		Schedulealgo(T);

		return temp;
	}
	void setstate(char z) {
		state = z;
	}
	void setrun(int T)
	{
		Schedulealgo(T);
		if (running != nullptr)
			state = 'B';
		else
			state = 'I';
	}

	char isbusy()
	{
		return state;
	}
	process* getrun() {
		return running;
	}
	virtual process* isfinished(int T)
	{
		if (state == 'S')
		{
			OT++;
			if (OT == n)
			{
				OT = 0;
				state = 'I';
			}
		}
		if (running != nullptr)
		{
			busytime++;

			running->incrementWON();
			process* temp = running;
			if (running->getWON() == running->getCT())
			{
				Schedulealgo(T);

				if (running != nullptr)
				{
					state = 'B';
					//timeleft = timeleft - running->getCT();
					running->setstate('RUN');

					if (running->getRT() == 0)
					{
						running->setRT(T);

					}
				}
				else
				{
					state = 'I';
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
	virtual void addtoready(process* pr) = 0;//adds to the list
	virtual void Schedulealgo(int T) = 0;
	virtual process* needsIO(int T)
	{
		process* temp = running;
		if (temp->getNIO() != 0)
		{
			if (running->getWON() == running->getIO_R())
			{
				Schedulealgo(T);
				if (running != nullptr) {
					state = 'B';
					running->setstate('RUN');
					if (running->getRT() == 0) {
						running->setRT(T);
					}
				}
				else
				{
					state = 'I';
				}
				temp->setstate('BLK');
				return temp;
			}
		}
		return nullptr;
	}
	//checks if WON is equal to its next IO_R,and if yes changes the running pointer to the its next process sets its state to R and sets its RT
	//and returns a pointer to the one that needs IO to be added to the blocked
	//if no returns a nullptr
	virtual void printdata() = 0;

	virtual int GetTimer()
	{
		return timeleft;
	}
	virtual int GetBusyTime()
	{
		return busytime;
	}

};
