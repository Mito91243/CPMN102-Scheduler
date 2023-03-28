#pragma once
#include "process.h"
#include <iostream>

using namespace std;

class Schudeler
{
private:
	
	//pointer to the GUI
	//Queue<*process> ReadyList
	//Queue<*process> RunList
	//Queue<*process> BlockedList
	//Queue<*process> Terminated
	//int number of FCFS
	//int number of SJF
	//int number of RR

public:

	//void AddProcess();  Chooses which process goes to which processor
	void LoadInput();
	void Run();
	//void ExecuteProcess(int x);  Executes process at current timestep
};
