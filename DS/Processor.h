#pragma once
#include"process.h"
class Processor
{
private:
	process* running; // pointer to what the process is working on
public:
	virtual process* isfinished() = 0;
	// increments WON and checks if it is equal to its CT,and if yes changes the running pointer to the its next process sets its state to R and sets its RT  
	//and returns a pointer to the finished one to be added to the termminated
	//if no returns a nullptr
	virtual void addtoready(process* pr) = 0;//adds to the list
	virtual process* needsIO() = 0; //checks if WON is equal to its next IO_R,and if yes changes the running pointer to the its next process sets its state to R and sets its RT
	//and returns a pointer to the one that needs IO to be added to the blocked
	//if no returns a nullptr
};
