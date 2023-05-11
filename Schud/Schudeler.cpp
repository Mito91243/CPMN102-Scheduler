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
    processornum = 0;
    LBnum = 0;
    processnum = 0;
    pArr = NULL;
    SigKill = NULL;
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
    processornum = nRR + nFCFS + nSJF;

    //Create Processor Array in the heap
    //Assign data member pArr to it
    Processor** arrp = new Processor * [processornum];
    pArr = arrp;

    //Read RR Slice number (Second Line)
    int RRslice;
    InFile >> RRslice;

    //Read RTF,MAXW,STL,FORK PROBABILITY
    int  Fork;
    InFile >> RTF >> MAXW >> STL >> Fork;


    int c = 0;

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
        process* temp = new process(AT, PID, CT, N);

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
    int PIDD, SIGKILL;
    InFile >> PIDD;
    LinkedList<int> pid;
    LinkedList<int> sig;

    pid.InsertBeg(PIDD);
    InFile >> SIGKILL;
    sig.InsertBeg(SIGKILL);

    while (!InFile.eof())
    {
        InFile >> PIDD >> SIGKILL;
        pid.InsertEnd(PIDD);
        sig.InsertEnd(SIGKILL);
    }

    // allocate memory for the rows
    int** arr = new int* [pid.count()];

    //Allocate memory for the columns
    for (int i = 0; i < pid.count(); ++i)
        arr[i] = new int[2];

    //Fill in the 2-D array
    for (int i = 0; i < pid.count(); i++)
    {
        arr[i][0] = pid.traverse(i);
        arr[i][1] = sig.traverse(i);
    }

    //Allocate it to data member and delete the 2 LinkedLists
    SigKill = arr;
    pid.DeleteAll();
    sig.DeleteAll();
}

void Schudeler::Run()
{
    LoadInput();
    

    if (processnum == 0 || processornum == 0)
        return;

    char x = 'y';
    while (x == 'y')
    {
        Simulate();
        Allocate(nullptr);
        
        pUI->updateUI(timestep, processornum, pArr, BlockedList, TerminatedList);
        //pUI->input();
        cin >> x;
        timestep++;

    }

    cout << "Output Created:  " << endl;
    Output();

}

void Schudeler::Terminate(process* p)
{
    TerminatedList.enqueue(p);
}

void Schudeler::Simulate()
{
    process* p;
    srand(time(NULL));
    for (int i = 0; i < processornum; i++)
    {
        if (pArr[i]->isbusy() == 'I')
        {
            pArr[i]->setrun();
        }

        int rnumber = rand() % 100 + 1;
        if (rnumber < 3)
        {
            pArr[i]->setstate('S');
            p = pArr[i]->changerun();
            while (p)
            {
                Allocate(p);
                p = pArr[i]->changerun();
            }
        }

        RRprocessor* R = dynamic_cast<RRprocessor*>(pArr[i]);
        if (R)
        {
            p = R->reachedts(timestep);
            if (p)
            {
                Allocate(p);
            }
        }

        p = pArr[i]->isfinished(timestep);
        if (p)
        {
            Terminate(p);
            orph(p);
        }


        if (pArr[i]->isbusy() == 'B')
        {
            p = pArr[i]->needsIO(timestep);
            if (p)
            {
                BlockedList.enqueue(p);
            }
        }

        FCFS* F = dynamic_cast<FCFS*>(pArr[i]);
        if (F)
        {
            for (int j = 0; j < nSK; j++)
            {
                if (SigKill[j][0] == timestep)
                {
                    process* k = F->kill(SigKill[j][1], timestep);
                    if (k)
                    {
                        Terminate(k);
                        orph(k);
                    }
                }
            }
            if (rnumber < 7 && F->isbusy() == 'B')
            {
                p = F->fork();
                if (p)
                {
                    process* temp = new process(timestep, ++processnum, (p->getCT() - p->getWON()), 0);
                    p->setparent(temp);
                    temp->setischild();
                    Allocate(temp);
                }
            }
        }

    }

    process* B = nullptr;
    if (BlockedList.peekFront(B))
    {
        B->incrementIOT();
        if (B->getIO_D() == B->getIOT())
        {
            B->incrementIO();
            B->resetIOT();
            BlockedList.dequeue(B);
            Allocate(B);
        }
    }

}

void Schudeler::Allocate(process*pr)
{
    int min = 9999;
    int max = 0;
    Processor* maxQueue= nullptr;
    Processor* minQueue = nullptr;
    process* p, * q;
    
    //Get Longest and shortest Queue
    for (int i = 0; i < processornum; i++)
    {
        
        if (pArr[i]->GetTimer() < min && pArr[i]->isbusy() != 'S')
        {
            minQueue = pArr[i];
            min = pArr[i]->GetTimer();
        }

    }
    
    //Allocate process rag3a mn Blocked to shortest Queue
    if (pr != nullptr)
    {  
        int minWT = 100000000000000000;
        int index = 0;
        
        //Check if process is child so it can go only to FCFS
        if (pr->getischild())
        {
            for (int i = 0; i < processornum; i++)
            {
                FCFS* F = dynamic_cast<FCFS*>(pArr[i]);
                if (pArr[i]->GetTimer() < minWT && pArr[i]->isbusy() != 'S' && F)
                {
                    minWT = pArr[i]->GetTimer();
                    index = i;
                }
            }
            pArr[index]->addtoready(pr);
        }
        else // process coming from blocked or overheating processor
        {
            minQueue->addtoready(pr);
        }
        //return to avoid redundant calls of Allocate
        return;
    }

    //condition executes when timestep is = or is a multiple of STL
    //Only check after first timestep cause 0%STL is runtime error 
    if (timestep != 0 && (timestep % STL) == 0)
    {
       // LoadBalancing();
    }

    //Normal ALlocation for Processor with shortest queue
        if (NewList.isEmpty())
            return;
    
        NewList.peekFront(p);
    
        //check if process in newList has arrival time as timestep
        if (!p || p->getAT() != timestep)
            return;

        minQueue->addtoready(p);

        //remove the process from newlist
        NewList.dequeue(q);

        pr = nullptr;
        //recursive call again to see if multiple processes has the same timestep
        Allocate(pr);
}

void Schudeler::LoadBalancing()
{
    int min = 9999;
    int max = 0;
    Processor* maxQueue = nullptr;
    Processor* minQueue = nullptr;
    process* q;

    for (int i = 0; i < processornum; i++)
    {

        if (pArr[i]->GetTimer() < min && pArr[i]->isbusy() != 'S')
        {
            minQueue = pArr[i];
            min = pArr[i]->GetTimer();
        }
        if (pArr[i]->GetTimer() > max)
        {
            maxQueue = pArr[i];
            max = pArr[i]->GetTimer();
        }
    }
    cout << "MAXQUEUE TIMELEFT IS : "<<maxQueue->GetTimer() << endl;
    cout << "MINQUEUE TIMELEFT IS : " << minQueue->GetTimer() <<endl;
    //get the ratio between longest queue and shortest queue with respect to CT
    int stl = (maxQueue->GetTimer() - minQueue->GetTimer()) / maxQueue->GetTimer();
    for (int i = 0; i < processornum; i++)
    {
        if (maxQueue == pArr[i])
        {
            cout << "MAXQUEUE IS : " << i << endl;
        }
        if (minQueue == pArr[i])
        {
            cout << "MinQueue IS: " << i << endl;
        }
    }
    
    cout << " CHECK 1" << endl;

    if (stl < 0.4)
        return;

    cout << "CHECK 2" << endl;
    process* p = maxQueue->Schedulealgo();
    minQueue->addtoready(p);
    //Counter for number of processes moved
    LBnum++;

    cout << "CHECK 3" << endl;
    //Recursive call to check that the updated Load Balance is under 0.4
    LoadBalancing();
}

void Schudeler::Output()
{
    ofstream OutFile;
    process* q = nullptr;
    
    OutFile.open("outputzz.txt");
    OutFile << "  TT       " << "PID     " << "AT      " << "CT      " << "IO_D " << "       WT" << "      RT" << "      TRT" << endl;

    int size = TerminatedList.getCount();

    process** prArr = new process* [size];
    
    for (int i = 0; i < size;i++)
    {
        TerminatedList.dequeue(q);
        prArr[i] = q;
    }


     //cout Each process in the output file 
    for(int i=0; i<size; i++)
    {

                 //printing details of each process
                 OutFile << "  " << prArr[i]->getTT() << "        " << prArr[i]->getPID() << "       " << prArr[i]->getAT() << "      " << prArr[i]->getCT() << "      ";

                 //This part handles if a process have multiple IO_D's to print them
                 //int* arr= prArr[i]->getIO_D();
                 //int zz=0;
                 //while (arr[zz] > 0)
                 //{
                 //    OutFile << arr[zz++] << " ";
                 //}
                 //arr = NULL;
           
                 //printing details of each process
                 OutFile << "           " << prArr[i]->getWT() << "      " << prArr[i]->getRT() << "       " << prArr[i]->getTRT() << endl;
    }
 
     

     //delete sorted array of TT 
         //delete[]arrS;
         //arrS = NULL;

         OutFile << endl << endl <<"Processes: " << size << endl;

         int sumRT = 0;
         int sumWT = 0;
         int sumTRT = 0;

         for (int i = 0; i < size; i++)
         {
             sumWT = sumWT + prArr[i]->getWT();
             sumRT = sumRT + prArr[i]->getRT();
             sumTRT = sumTRT + prArr[i]->getTRT();
         }

         OutFile << "Avg WT = " << sumWT / size << "     " << "Avg RT = " << sumRT / size << "     " << "Avg TRT = " << sumTRT / size << endl;
         
         
         //OutFile << "Migration %:   RTF =  " << RTFpercentage << " %" << "   MAXW =  " << MAXWpercentage << " %" << endl;
         
         OutFile << "Work Steal %:  " << LBnum/processnum << endl;
         //OutFile << "Forked Process %:  " << Fork percentage << endl;
         //OutFile << "Killed Process %:  " << Kill percentage << endl;
  
         
         //get the number of each processor type and print it
         int nFCFS = 0;
         int nRR = 0;
         int nSJF = 0;
         for (int i = 0; i < processornum; i++)
         {
             FCFS* F = dynamic_cast<FCFS*>(pArr[i]);
             RRprocessor* R = dynamic_cast<RRprocessor*>(pArr[i]);
             SJF* S = dynamic_cast<SJF*>(pArr[i]);
             if (F)nFCFS++;
             if (R)nRR++;
             if (S)nSJF++;
         }

         OutFile << "Processors: " << processornum << "  [ " << nFCFS << "  FCFS , " << nSJF << "  SJF , " << nRR << "  RR" << " ]" << endl;
         
         
         //Processor Load OutPut
         OutFile << "Processors Load " << endl;

         int TRT;
         for (int i = 0; i < processornum; i++)
         {
             int busytime = pArr[i]->getIDLE(TRT);                       
             
             ////////////////////Termination Time always 0
             cout << "TRT IS : " << TRT << endl;
             
             //a check to make sure busytime and TRT are not 0 to avoid run errors
             if (busytime == 0 || TRT == 0)
                 continue;

             cout << "Busy Time is : " << busytime << endl;

             int pLoad = busytime / TRT;

             OutFile << "P" << i << "= " << pLoad << " % ";
         }

         //Processor Utilization Output
         OutFile << endl << "Processors Utilization " << endl;

         int sumUtilz = 0;
         for (int i = 0; i < processornum; i++)
         {
             int busytime = pArr[i]->getIDLE(TRT);
             
             if (busytime == 0)
                 continue;



             float pUtilz = busytime / timestep;
             sumUtilz = sumUtilz + pUtilz;

             OutFile << "P" << i << "= " << pUtilz << " % ";
         }

         //Average Utilization
         int avgUtilz = sumUtilz / processornum;

         OutFile << endl << "Avg utilization: " << avgUtilz << " %" << endl;

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
    //delete the processors
    for (int i = 0; i < processornum; i++)
    {
        delete pArr[i];
        pArr[i] = NULL;
    }
    //delete the SigKill
    //TO be written in phase2

}

void Schudeler::orph(process* p)
{
    if (p->getlchild())
    {
        bool found = false;
        for (int j = 0; j < processornum; j++)
        {
            FCFS* F = dynamic_cast<FCFS*>(pArr[j]);
            if (F)
            {
                found += F->orphan(p->getlchild(), timestep);
            }
        }
        if (found)
        {
            Terminate(p->getlchild());
            orph(p->getlchild());
        }
    }
    if (p->getrchild())
    {
        bool found = false;
        for (int j = 0; j < processornum; j++)
        {
            FCFS* F = dynamic_cast<FCFS*>(pArr[j]);
            if (F)
            {
                found += F->orphan(p->getrchild(), timestep);
            }
        }
        if (found)
        {
            Terminate(p->getrchild());
            orph(p->getrchild());
        }
    }
}
