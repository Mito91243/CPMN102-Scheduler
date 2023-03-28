#include "process.h"
#include <iostream>
using namespace std;

process::process(int A, int I, int C, int NO) 
{
	AT = A;
	PID = I;
	CT = C;
	NIO = NO;
	countIOR = 0;
	countIOD = 0;
	nextIO = 0;
	WON = 0;
	AIO_D = new int[NIO];
	AIO_R = new int[NIO];
	cout << "Process Created: " << endl;
}
void process::setRT(int T) {
	RT = T - AT;
}
void process::setTT(int T) {
	TT = T;
}
void process::setTRT() {
	TRT = TT - AT;
}
void process::setWT() {
	WT = TRT - CT;
}
void process::setstate(char a) {
	state = a;
}
void process::setparent(process* ch) {
	parent = true;
	child = ch;
}
void process::AddIO_R(int x) {
	AIO_R[countIOR] = x;
	countIOR++;
	cout << "IOR added Successully" << endl;
}
void process::AddIO_D(int x) {
	AIO_D[countIOD] = x;
	countIOD++;
	cout << "IOD added succefully" << endl;
}
void process::incrementIO() {
	nextIO++;
}
void process::incrementWON() {
	WON++;
}
int process::getAT() {
	return AT;
}
int process::getPID() {
	return PID;
}
int process::getCT() {
	return CT;
}
int process::getRT() {
	return RT;
}
int process::getTT() {
	return TT;
}
int process::getTRT() {
	return TRT;
}
int process::getWT() {
	return WT;
}
char process::getstate() {
	return state;
}
bool process::isparent() {
	return parent;
}
process* process::getchild() {
	return child;
}
int process::getWON() {
	return WON;
}
int process::getIO_R() {
	return AIO_R[nextIO];
}
int process::getIO_D() {
	return AIO_D[nextIO];
}
process::~process() {
	delete[]AIO_D;
	delete[]AIO_R;
}