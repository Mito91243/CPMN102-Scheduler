#pragma once
#include"PriorityQueue.h"
#include"Processor.h"
class SJF :public Processor
{
private:
	PriorityQueue<process*>  sjfqueue;

public:
	SJF(Schudeler* sch);
	void addtoready(process* pr);
	void Schedulealgo(int T);
	process* getrun();
	void printdata();
};
