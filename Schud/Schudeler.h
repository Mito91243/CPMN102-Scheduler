#include "Node.h"
#include <iostream>

using namespace std;

class Schudeler
{
private:
	
	Queue<process*> NewList;
	//Queue<process*> ReadyList; -- > Each processor got 1
	SJF ps;
	Queue<process*> BlockedList;
	Queue<process*> TerminatedList;
	
	int timestep = 0;
	int nFCFS;
	int nSJF;
	int nRR;

public:

	template<typename A, typename B>
	void MoveProcess(A Old, B New);
	
	template<typename A, typename B>
	void MoveProcessor(A Old, B New);

	void PrintInfo();
	void Execute(int timestep);  //Chooses which process goes to which processor
	void LoadInput();
	void Run();
	void Allocate();
};
