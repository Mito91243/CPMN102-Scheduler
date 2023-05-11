#pragma once
#include "process.h"
#include "Processor.h"
#include "SJF.h"
#include "Queue.h"
#include "Node.h"
#include "UI.h"
#include "LinkedList.h"
#include <iostream>

using namespace std;

class Schudeler
{
private:
	
	UI*pUI;
	Queue<process*> NewList;
	Queue<process*> BlockedList;
	Queue<process*> TerminatedList;
	Processor** pArr;
	int processornum;
	int processnum;
	int timestep;
	int** SigKill;

	int STL;
	int RTF;
	int MAXW;
	int LBnum;

	int nSK;


public:

\
	Schudeler();
	template<typename A, typename B>
	void MoveProcessor(A Old, B New);
	void Terminate(process* p);
	void PrintInfo();
	void LoadInput();
	void Run();
	void Allocate(process* pr);
	void Output();
	
	//void Migration();
	void LoadBalancing();
	void Simulate();
	void orph(process*p);
	~Schudeler();
};



