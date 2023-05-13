#pragma once
#ifndef _FCFS
#define _FCFS

#include "LinkedList.h"
#include "Processor.h"

class FCFS : public Processor
{
private:
	LinkedList<process*> RL;
	int MAXW;
public:
	FCFS(int maxw, Schudeler* sch);
	void addtoready(process* pr);
	void Schedulealgo(int T);
	static bool pidcheck(process* pr, int p);
	process* kill(int r, int t);
	void printdata();
	process* getrun();
	process* fork();
	bool orphan(process* p, int t);
};

#endif	
