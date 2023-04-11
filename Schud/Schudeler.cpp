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
    int totalprocessors = FCFS + SJF + RR;

    //Create Processor Array in the heap
    //Assign data member Arrp to it
     Processor** arrp = new Processor*[totalprocessors];
     Arrp = arrp;

    //Read RR Slice number (Second Line)
    int RRslice;
    InFile >> RRslice;

    //Read RTF,MAXW,STL,FORK PROBABILITY
    int RTF, MaxW, STL, Fork;
    InFile >> RTF >> MaxW >> STL >> Fork;


    //Read the number of processes
    int ProcessNum;
    InFile >> ProcessNum;

    //Read Each proccess details
    int PID, AT, CT, N;
    for (int i = 0; i < ProcessNum; i++)
    {
        InFile >> AT >> PID >> CT >> N;
        cout << AT << endl;
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
        cout << "PID is: " << PIDD << endl;
        cout << "SIGKILL IS : " << SIGKILL << endl;
        KID.enqueue(PIDD);
        Ktime.enqueue(SIGKILL);
        cout << "Done Succesffffulll" << endl;
        cout << Ktime.dequeue(xx) << endl;
        
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

       // Execute(timestep);
        PrintInfo();

        
        
        
        
        cout << "Press y to move to next step! " << endl;
        cin >> x;

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

void Schudeler::PrintInfo()
{


   // cout << "--------------------------- RDY processes ---------------------------" << endl;
   // cout << "Processor 1 [FCFS]: 0 RDY: " << endl; //print each ID of the RDY lists
   // cout << "Processor 2 [SJF]: 0 RDY" << endl;  // print no. of ready in each processor
   // cout << "Processor 3 [RR] : 0 RDY" << endl;

   // cout << "--------------------------- BLK processes ---------------------------" << endl;
   // cout << BlockedList.getCount() << " BLK: " << endl; //print total blocked processes with their ID's

   // cout << "--------------------------- RUN processes ---------------------------" << endl;
   //// cout << "3 RUN " << endl; // print total running process with their ID's and processor ex: 21 (p3)

   // cout << "--------------------------- TRM processes ---------------------------" << endl;
   // cout << TerminatedList.getCount() <<" TRM: " << endl; // print each process ID
}



void Schudeler::Allocate()
{
    //Generate Random Number
    int rnumber = rand() % 100 + 1;

    //Case for Moving from Lists mentioned in Phase 1
    if (1 < rnumber < 15)
    {
        //MoveProcess(NewList , BlockedList);
    }
    else if(20 < rnumber < 30)
    {
        //MoveProcess(RunList of processor , ReadyList of processor);  
    }
    else if (50 < rnumber < 60)
    {
        //MoveProcess(RunList of processor , Terminated List);
    }
    else if (20 < rnumber < 30)
    {
        //MoveProcess(RunList of processor , ReadyList of processor)   The ---- should be the RunQueue in the processor
    }

    int rnumber2 = rand() % 100 + 1;
    process* p;
    if (rnumber2 < 10)
    {
        BlockedList.dequeue(p);
        //Move to ready List of processor
    }

}
  
 
