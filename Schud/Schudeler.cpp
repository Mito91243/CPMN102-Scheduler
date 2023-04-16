#include <iostream>
#include <fstream>
#include "Schudeler.h"
#include "RRprocessor.h"
#include "LinkedList.h"
#include "SJF.h"
#include "FCFS.h"
#include "Node.h"
#include "process.h"
#include <string.h>
#include <windows.h>
#include "UI.h"

using namespace std;


Schudeler::Schudeler()
{
    timestep = 0;
    Acounter = 0;
    processornum = 0;
    processnum = 0;
    pArr = NULL;

    pUI = new UI;

}

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
    int nFCFS, nSJF, nRR;
    InFile >> nFCFS >> nSJF >> nRR;
    processornum = nRR + nFCFS + nSJF ;

    //Create Processor Array in the heap
    //Assign data member pArr to it
     Processor** arrp = new Processor*[processornum];
     pArr = arrp;

    //Read RR Slice number (Second Line)
    int RRslice;
    InFile >> RRslice;

    //Read RTF,MAXW,STL,FORK PROBABILITY
    int RTF, MaxW, STL, Fork;
    InFile >> RTF >> MaxW >> STL >> Fork;


    int c=0;

    for (int i = 0; i < nRR; i++)
    {
        pArr[i] = new RRprocessor(RRslice);
    }
    for (int i = 0; i < nFCFS; i++)
    {
        pArr[nRR + i] = new FCFS();

    }
    for (int i = 0; i < nSJF; i++)
    {
        pArr[nRR + nFCFS + i] = new SJF();

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
    //int PIDD, SIGKILL;
    //InFile >> PIDD;
    //PIDs.InsertBeg(PIDD);
    //InFile >> SIGKILL;
    //SIGs.InsertBeg(SIGKILL);

    //while (!InFile.eof())
    //{
    //    InFile >> PIDD >> SIGKILL;
    //        PIDs.InsertEnd(PIDD);
    //        SIGs.InsertEnd(SIGKILL);     
    //}

}
void Schudeler::Run()
{
    LoadInput();
    
    char x = 'y';

    while(x == 'y')

    {
        Sleep(1000);

        cout << "Current Timestep :  " << timestep << endl;

        Simulate();
        Allocate();

        pUI->updateUI(timestep, pArr, BlockedList, TerminatedList);
        pUI->input();
        //cin >> x;
        timestep++;
        
    }

}

//Move from processor to another processor
//template<typename A, typename B>
//void Schudeler::MoveProcessor(A Old, B New)
//{
//    process* p;
//    p = Old.Schedulealgo(); // Change implementation schdule algo
//    New.addtoready(p);
//}

void Schudeler::Terminate(process *p)
{
    TerminatedList.enqueue(p);
}



void Schudeler::Simulate()
{
    process* p;
    srand(time(0));
    int *rrnum = new int;
    for (int i = 0; i < processornum; i++)
    {

        
        //Generate Random Number
        int rnumber = rand() % 100 + 1;
        
        //Check if randomized number is within range
        if (rnumber > 60 || (rnumber > 15 && rnumber < 20) || (rnumber > 30 && rnumber < 50) || (20 < rnumber && rnumber < 30))
            continue;

        //Get Running Process
         p = pArr[i]->changerun(timestep);

         //Check if running process is not NULL
         if (!p)
            continue;


        //Cases
        if (1 <= rnumber && rnumber <= 15)
        {
            *rrnum = rnumber;
            BlockedList.enqueue(p);
        }
        //else if (20 < rnumber && rnumber < 30) // WONT WORK BECAUSE IT TAKES THE RUN AND ADD IT TO READY AND READY FUNC MAKES THE PROCESS HEAD TO RUN
        //{
        //    pArr[i]->addtoready(p,timestep);
        //}
        else if (50 <= rnumber && rnumber <= 60)
        {
            *rrnum = rnumber;
            TerminatedList.enqueue(p);

        }

    }
   
    //check if blockedList is empty
    if (BlockedList.isEmpty())
       return;

    srand(timestep);
    //Case 2

    int rnumber2 = rand() % 100 + 1;
    if (rnumber2 <= 10 && *rrnum > 1 && *rrnum <= 15)
        return;

    if (rnumber2 <= 10)
    {
            BlockedList.dequeue(p);
            TerminatedList.enqueue(p);
    }

}

void Schudeler::Allocate()
{
    process* p,*q;

    if (NewList.isEmpty())
        return;

     NewList.peekFront(p);

     //Once counter reaches processornum reset
    if (Acounter == processornum)
        Acounter = 0;

    
    //check if process in newList has arrival time as timestep
    if (!p || p->getAT() != timestep)
        return;
    
    pArr[Acounter++]->addtoready(p, timestep);
    
        //remove the process from newlist
        NewList.dequeue(q);
        //recursive call again to see if multiple processes has the same timestep
        Allocate();  
}

Schudeler::~Schudeler()
{
    delete pUI;
    pUI = NULL;

    int counter = 0;
    while (pArr[counter])
    {
        counter++;
    }

    for (int i = 0; i < counter; i++)
    {
        delete pArr[i];
        pArr[i] = NULL;
    }


}
 
