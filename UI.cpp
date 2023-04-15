#pragma once
#include "process.h"
#include "Processor.h"
#include "FCFS.h"
#include"stdlib.h"
#include "RRprocessor.h"
#include "SJF.h"
#include "Queue.h"
#include "Schudeler.h"
#include <iostream>
using namespace std;

class UI 
{
public:
	UI()
	{
		Schudeler* S = new Schudeler;
		S->Run();
	}
	void updateUI(int &t,Processor* P[],Queue<process*> &B,Queue<process*> &T)
	{
		system("cls");
		cout << "Current Timestep:" << t << endl;
		cout << '\t' << "RDY processes" << '\t' << endl;
		for (int i = 0; i < sizeof(P); i++)
		{
			cout << "processor " << i;
			FCFS* F = dynamic_cast<FCFS*>(P[i]);
			RRprocessor* R = dynamic_cast<RRprocessor*>(P[i]);
			SJF* S = dynamic_cast<SJF*>(P[i]);
			if (F)cout << " [FCFS]: ";
			if (R)cout << " [RR]: ";
			if (S)cout << " [SJF]: ";
			P[i]->printdata();
			cout << endl;
		}
		cout << '\t' << "BLK processes" << '\t' << endl;
		cout << B.getCount() << " BLK: ";
		B.printq();
		cout << endl;
		cout << '\t' << "RUN processes" << '\t' << endl;
		int c = 0;
		for (int i = 0; i < sizeof(P); i++)
		{
			if (P[i]->getrun())
			{
				c++;
			}
		}
		cout << c << " RUN:";
		for (int i = 0; i < sizeof(P[i]); i++)
		{
			if (P[i]->getrun())
			{
				cout << P[i]->getrun()<<"(P"<<i<<") ";
			}
		}
		cout << endl;
		cout << '\t' << "TRM processes" << '\t' << endl;
		cout << T.getCount() << " TRM: ";
		T.printq();
		cout << endl;
	}

	void input()
	{
		system("pause");
		system("cls");
	}


};
