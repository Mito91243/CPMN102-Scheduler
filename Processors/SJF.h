#pragma once
#include"PriorityQueue.h"
#include"Processor.h"
class SJF :public Processor
{
private:
	int count;
	process* running;
	
	PriorityQueue<process*>  sjfqueue;
	int t;
public:
	SJF();
	void  addtoready(process* pr, int T);
	
	process* Schedulealgo();
	process* getrun();
	
	
};

