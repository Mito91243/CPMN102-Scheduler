#include <iostream>
#include <fstream>
#include "Schudeler.h"
#include "RRprocessor.h"
#include "process.h"
#include <string.h>
#include <windows.h>

using namespace std;

void Schudeler::LoadInput()
{


    //Open the File .txt
    string p;
    cin >> p;

    ifstream InFile("C:\\Users\\HP User\\Desktop\\Project Data\\" + p + ".txt");

    //Checks if the File is opened
    if (InFile.fail())
    {
        cout << "Error Opening the file: " << endl;
        exit(1);
    }



    //Read number of each Processor (First Line)
    int FCFS, SJF, RR;
    InFile >> FCFS >> SJF >> RR;
    processornum = RR ;

    //Create Processor Array in the heap
    //Assign data member Arrp to it
     Processor** arrp = new Processor*[processornum];
     Arrp = arrp;

    //Read RR Slice number (Second Line)
    int RRslice;
    InFile >> RRslice;

    //Read RTF,MAXW,STL,FORK PROBABILITY
    int RTF, MaxW, STL, Fork;
    InFile >> RTF >> MaxW >> STL >> Fork;


    for (int i = 0; i < RR; i++)
    {
        Arrp[i] = new RRprocessor(RRslice);
    }

    //Read the number of processes
   
    InFile >> processnum;

    //Read Each proccess details
    int PID, AT, CT, N;
    for (int i = 0; i < processnum; i++)
    {
        InFile >> AT >> PID >> CT >> N;

        //create a temporary pointer to the new process
        process *temp = new process(AT, PID, CT, N);

        
        //Read the IO_R and the IO_D
        if (N != 0)
        {
            InFile.ignore(1, ' ');
            int x, y;
            for (int i = 0; i < N; i++)
            {
                if (i != 0)
                {
                    InFile.ignore(1, ',');
                }

                InFile.ignore(1, '(');
                InFile >> x;
                InFile.ignore(1, ',');
                InFile >> y;
                InFile.ignore(1, ')');

                //Add the IO_D and IO_R incrementally
                temp->AddIO_D(x);
                temp->AddIO_R(y);
                
            }
        }

        //Add each process to the readylist
        NewList.enqueue(temp);
        
    }


    //Read the SIGKILL and the PID
    int PIDD, SIGKILL,xx;
    while (InFile.peek() != EOF)
    {
        InFile >> PIDD >> SIGKILL;
        
    }

    InFile.close();

}
void Schudeler::Run()
{
    cout << "Choose Input File: " << endl;
    LoadInput();
    
    char x = 'y';

    while(x == 'y')

    {
        Sleep(1000);

        cout << "Current Timestep :  " << timestep << endl;

        Allocate();
        Simulate();

        
        
        
        
        cout << "Press y to move to next step! " << endl;
        cin >> x;

        // process*p = Processor.is finished
        //
        timestep++;
        
    }

}

void Schudeler::Execute(int timestep)
{
}

//Move From processor to BLK LIST / TRM LIST
template<typename A,typename B>
void Schudeler::MoveProcess(A Old,B New)
{
    process *p;
    p = Old.Schedulealgo();
    New.enqueue(p);
}

//Move from processor to another processor
template<typename A, typename B>
void Schudeler::MoveProcessor(A Old, B New)
{
    process* p;
    p = Old.Schedulealgo();
    New.addtoready(p);
}


void Schudeler::MoveToTerminated(process *p)
{
    TerminatedList.enqueue(p);
}

void Schudeler::PrintInfo()
{




   // cout << "--------------------------- RDY processes ---------------------------" << endl;
   // cout << "Processor 1 [FCFS]: 0 RDY: " << endl; //print each ID of the RDY lists
   // cout << "Processor 2 [SJF]: 0 RDY" << endl;  // print no. of ready in each processor
    //cout << "Processor 3 [RR] :  " << R

   // cout << "--------------------------- BLK processes ---------------------------" << endl;
   // cout << BlockedList.getCount() << " BLK: " << endl; //print total blocked processes with their ID's

   // cout << "--------------------------- RUN processes ---------------------------" << endl;
   //// cout << "3 RUN " << endl; // print total running process with their ID's and processor ex: 21 (p3)

   // cout << "--------------------------- TRM processes ---------------------------" << endl;
   // cout << TerminatedList.getCount() <<" TRM: " << endl; // print each process ID
}



void Schudeler::Simulate()
{
    process* p;
    srand(time(0));

    for (int i = 0; i < processornum; i++)
    {

        //Get the current processor Run
        p = Arrp[i]->changerun(timestep);

        //if the processor has no running process return and move on to next process
        if (!p)
            return;


        //Generate Random Number
        int rnumber = rand() % 100 + 1;

        //Cases
        if (1 < rnumber && rnumber < 15)
        {
            BlockedList.enqueue(p);
        }
        else if (20 < rnumber && rnumber < 30)
        {
            Arrp[i]->addtoready(p,timestep);
        }
        else if (50 < rnumber && rnumber < 60)
        {
            TerminatedList.enqueue(p);
        }


    }
   
    //check if blockedList is empty
    if (BlockedList.isEmpty())
       return;

    int rnumber = rand() % 100 + 1;

    if (rnumber <= 10)
    {
            BlockedList.dequeue(p);
    }

}

void Schudeler::Allocate()
{
    process* p,*q;
    NewList.peekFront(p);
    
    //check if process in newList has arrival time as timestep
    if (!p || p->getAT() != timestep)
        return;
    
        //loop over processors to see which one has no running process meaning it's empty
    int min = 99999999;
    //get the smallest readylist
    for (int i = 0; i < processornum; i++)
    {
        if (Arrp[i]->getRDYCount() < min)
        {
            min = Arrp[i]->getRDYCount();
        }

    }
    //get the processor with that smallest list and assign it process
    for (int i = 0; i < processornum; i++)
    {
        if (Arrp[i]->getRDYCount() == min)
        {
            Arrp[i]->addtoready(p,timestep);
        }
    }

        //remove the process from newlist
        NewList.dequeue(q);
        //recursive call again to see if multiple processes has the same timestep
        Allocate();  
}
 
