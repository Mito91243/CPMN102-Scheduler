#include <iostream>
#include <fstream>
#include "Schudeler.h"
#include "RRprocessor.h"
#include "SJF.h"
#include "FCFS.h"
#include "process.h"
#include <string.h>
#include <windows.h>

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
    cout << "PROCESSOR NUM IS: " << processornum<< endl;
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

        Simulate();
        Allocate();
        PrintInfo();
        
        
        
        
        cout << "Press y to move to next step! " << endl;
        cin >> x;

        


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


void Schudeler::MoveToTerminated(process *p)
{
    TerminatedList.enqueue(p);
}

void Schudeler::PrintInfo()
{


    cout << "--------------------------- RDY processes ---------------------------" << endl;
    cout << "Processor 1 [FCFS]: 0 RDY: " << endl; //print each ID of the RDY lists
    cout << "Processor 2 [SJF]: 0 RDY" << endl;  // print no. of ready in each processor
    cout << "Processor 3 [RR] : 0 RDY " << endl;

    cout << "--------------------------- BLK processes ---------------------------" << endl;
    cout << BlockedList.getCount() << " BLK: " << endl; //print total blocked processes with their ID's

    cout << "--------------------------- RUN processes ---------------------------" << endl;
   // cout << "3 RUN " << endl; // print total running process with their ID's and processor ex: 21 (p3)

    cout << "--------------------------- TRM processes ---------------------------" << endl;
    cout << TerminatedList.getCount() <<" TRM: " << endl; // print each process ID
}



void Schudeler::Simulate()
{
    process* p;
    srand(time(0));

    for (int i = 0; i < processornum; i++)
    {


        //Generate Random Number
        int rnumber = rand() % 100 + 1;

        //Check if randomized number is within range
        if (rnumber > 60 || (rnumber > 15 && rnumber < 20) || (rnumber > 30 && rnumber < 50))
            continue;
        
        //Get Running Process
         p = pArr[i]->getrun();

         //Check if runnging process is not NULL
         if (!p)
            continue;
       

        //Cases
        if (1 < rnumber && rnumber < 15)
        {
            BlockedList.enqueue(p);
        }
        else if (20 < rnumber && rnumber < 30)
        {
            pArr[i]->addtoready(p,timestep);
        }
        else if (50 < rnumber && rnumber < 60)
        {
            TerminatedList.enqueue(p);
        }


    }
   
    //check if blockedList is empty
    if (BlockedList.isEmpty())
       return;

    //Case 2
    int rnumber2 = rand() % 100 + 1;
    if (rnumber2 <= 10)
    {
            BlockedList.dequeue(p);
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
 
