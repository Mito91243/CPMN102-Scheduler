#pragma once
#include<iostream>
using namespace std;
class process
{
private:
	int AT;
	int PID;
	int CT;
	int NIO;
	int* AIO_R;
	int countIOR;
	int* AIO_D;
	int countIOD;
	int nextIO;
	int RT;
	int TT;
	int TRT;
	int WT;
	int WON;  // time any processor has been working on this process, gets incremented every time step ny is finished of each processor
	char state;
	bool ischild;//true if process is a child false otherwise
	process* lch;
	process* rch;
	int IOT;
public:
	process(int A, int I, int C, int NO);
	void setRT(int T);
	void setTT(int T);
	void setTRT();
	void setWT();
	void setstate(char a);
	void setparent(process* ch);
	void AddIO_R(int x);
	void AddIO_D(int x);
	void incrementIO();
	void incrementWON();
	int getAT();
	int getPID();
	int getCT();
	int getRT();
	int getTT();
	int getTRT();
	int getWT();
	int getIO();
	int getNIO();
	char getstate();
	int getIO_R();
	int getIO_D();
	int getWON();
	process* getlchild();
	process* getrchild();
	void incrementIOT();
	int getIOT();
	void resetIOT();
	void setischild();
	bool getischild();
	~process();
	friend ostream& operator << (ostream& out, const process& p) {
		out << p.PID << "  ";
		return out;
	}
	bool operator ==(int k);
};


