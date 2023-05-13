#pragma once
#include"Processor.h"
#include"process.h"
#include"Queue.h"

class RRprocessor : public Processor
{
private:
	int timeslice;
	Queue<process*> Q1;
	int TOP;
	int RTF;
public:

	RRprocessor(int ts, int rt, Schudeler* sch);
	virtual void addtoready(process* pr);
	virtual void Schedulealgo(int T);
	process* reachedts(int T);
	Queue<process*> getqueue();
	void printdata();
	process* getrun();
};
