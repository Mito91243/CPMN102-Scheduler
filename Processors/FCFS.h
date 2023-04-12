#ifndef _FCFS
#define _FCFS

#include "LinkedList.h"
#include "Processor.h"

class FCFS : public Processor
{
private:

	LinkedList<process*> RL;
public:
	 void addtoready(process* pr);//adds to the list
	 process* Schedulealgo();
	 process* randkill(int r);
	 void print();
};

#endif	
