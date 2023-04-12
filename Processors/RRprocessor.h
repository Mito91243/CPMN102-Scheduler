#pragma once
#include"Processor.h"
#include"process.h"
#include"Queue.h"

class RRprocessor: public Processor
{
private:
	int timeslice;
	Queue<process*> Q1;
	process* running;
	int TOP;
public:
	RRprocessor(int ts);
	virtual void addtoready(process* pr,int T);
	virtual process* Schedulealgo();
	process* reachedts(int T);
	Queue<process*> getqueue();
	void printdata();
};

