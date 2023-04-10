#pragma once
#include"PriorityQueue.h"
#include"Processor.h"
class SJF :public Processor
{
private:
	int count;
	
	PriorityQueue<process*>  sjfqueue;
	int t;
public:
	SJF();
	void  addtoready(process* pr);
	
	process* Schedulealgo();
	
	
};

