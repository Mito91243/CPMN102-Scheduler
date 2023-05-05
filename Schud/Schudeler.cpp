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
        arr[i] = new int[pid.count()];

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

    while (TerminatedList.getCount() != processnum)
    {

        Simulate();
        Allocate();
        
        pUI->updateUI(timestep, processornum, pArr, BlockedList, TerminatedList);
        pUI->input();
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

void Schudeler::Terminate(process* p)
{
    TerminatedList.enqueue(p);
}

void Schudeler::incIO()
{
    if (BlockedList.isEmpty()) {
        return;  // Base case: if the BlockedList is empty, stop the recursion
    }

    process* p;
    BlockedList.dequeue(p);

    incIO();  // Recursive call to process the rest of the queue

    BlockedList.enqueue(p);  // Enqueue the dequeued process back into the queue

    // Move all the elements from the front of the queue to the back,
    // except for the process that was just dequeued and enqueued back
    // (this ensures that the original order is maintained)
    int size = BlockedList.getCount();
    for (int i = 0; i < size - 1; i++) {
        BlockedList.dequeue(p);
        BlockedList.enqueue(p);
    }

    p->incrementIOT();  // Increment the I/O time of the process
}



void Schudeler::shortestqueue(process* p)
{
    int minWT = 100000000000000000;
    int index = 0;
    for (int i = 0; i < processornum; i++)
    {
        if (p->getischild())
        {
            FCFS* F = dynamic_cast<FCFS*>(pArr[i]);
            if (pArr[i]->getWT() < minWT && pArr[i]->isbusy() != 'S' && F)
            {
                minWT = pArr[i]->getWT();
                index = i;
            }
        }
        else
        {
            if (pArr[i]->getWT() < minWT && pArr[i]->isbusy() != 'S')
            {
                minWT = pArr[i]->getWT();
                index = i;
            }
        }
    }
    pArr[index]->addtoready(p);
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



void Schudeler::Simulate()
{
    process* p;
    srand(time(NULL));
    for (int i = 0; i < processornum; i++)
    {
        if (pArr[i]->isbusy()=='I')
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
                shortestqueue(p);
                p = pArr[i]->changerun();
            }
        }

        RRprocessor* R = dynamic_cast<RRprocessor*>(pArr[i]);
        if (R)
        {
            p = R->reachedts(timestep);
            if (p)
            {
                shortestqueue(p);
            }
        }

        p = pArr[i]->isfinished(timestep);
        if (p)
        {
            Terminate(p);
            orph(p);
        }


        if (pArr[i]->isbusy()=='B')
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
                    process* k= F->kill(SigKill[j][1],timestep);
                    if (k)
                    {
                        Terminate(k);
                        orph(k);
                    }
                }
            }
            if (rnumber < 7 && F->isbusy()=='B')
            {
                p = F->fork();
                if (p)
                {
                    process* temp = new process(timestep, ++processnum, (p->getCT() - p->getWON()), 0);
                    p->setparent(temp);
                    temp->setischild();
                    shortestqueue(temp);
                }
            }
        }

    }

    process* B=nullptr;
    if (BlockedList.peekFront(B))
    {
        B->incrementIOT();
        if (B->getIO_D() == B->getIOT())
        {
            B->incrementIO();
            B->resetIOT();
            BlockedList.dequeue(B);
            shortestqueue(B);
        }
    }

}

void Schudeler::Allocate()
{
    int min = 9999;
    int max = 0;
    Processor* maxQueue= nullptr;
    Processor* minQueue = nullptr;
    process* p, * q;
    
    //Get Longest and shortest Queue
    for (int i = 0; i < processornum; i++)
    {
        
        if (pArr[i]->GetTimer() < min)
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


    //condition executes when timestep is = or is a multiple of STL
    //Only check after first timestep cause 0%STL is runtime error 
    if(timestep != 0 && (timestep % STL) ==0 )
        LoadBalancing(minQueue, maxQueue);
    

    //Normal ALlocation for Processor with shortest queue
        if (NewList.isEmpty())
            return;
    
        NewList.peekFront(p);
    
        //check if process in newList has arrival time as timestep
        if (!p || p->getAT() != timestep)
            return;

        minQueue->addtoready(p, timestep);

        //remove the process from newlist
        NewList.dequeue(q);

        //recursive call again to see if multiple processes has the same timestep
        Allocate();
}

void Schudeler::LoadBalancing(Processor* minQ, Processor* maxQ)
{
    //get the ratio between longest queue and shortest queue with respect to CT
    int stl = (maxQ->GetTimer() - minQ->GetTimer()) / maxQ->GetTimer();

    if (stl >= 0.4)
    {
        process* p = maxQ->Schedulealgo();
        minQ->addtoready(p,timestep);                                                           
    }

    //Recursive call to check that the updated Load Balance is under 0.4
    LoadBalancing(minQ,maxQ);
}

//void Schudeler::Migration()
//{
//    for (int i = 0; i < processornum; i++)
//    {
//        FCFS* F = dynamic_cast<FCFS*>(pArr[i]);
//        RRprocessor* R = dynamic_cast<RRprocessor*>(pArr[i]);
//
//        if (F)
//        {
//
//        }
//
//        if (R)
//        {
//            
//        }
//    }
//}

void Schudeler::Output()
//{
//    ofstream OutFile;
//    process* q = nullptr;
//    
//    OutFile.open("outputzz.txt");
//    OutFile << "  TT       " << "PID     " << "AT      " << "CT      " << "IO_D " << "       WT" << "      RT" << "      TRT" << endl;
//
//    int size = TerminatedList.getCount();
//
//
//    int* arrS = new int[size];
//    process** prArr = new process* [size];
//                              
//    
//    for (int i = 0; i < size;i++)
//    {
//        TerminatedList.dequeue(q);
//        arrS[i] = q->getCT();
//        prArr[i] = q;
//    }
//
//    //This struct has 3 functions that make quick sorting 
//    struct Sort
//    {
//        void swap(int &a,int &b)
//        {
//            int temp = a;
//            a = b;
//            b = temp;
//
//        }
//
//        int pivot(int* arrS, int start, int end)
//        {
//            int piv= arrS[end];
//            int indx = start;
//            
//            //Makes all numbers less than pivot on the it's left
//            for (int i = start; i < end; i++)
//            {
//                if (arrS[i] <= piv)
//                {
//                    swap(arrS[i], arrS[indx]);
//                    indx++;
//                }
//            }
//
//            swap(arrS[indx], arrS[end]);
//            return indx;
//
//        }
//
//        void QuickSort(int* arrS, int start, int end)
//        {
//            //always execute unless start becomes negative 
//            if (start < end)
//            {
//                //break array into 2 parts
//                int p = pivot(arrS, start, end);
//                //Recursive calling for left segment
//                QuickSort(arrS, 0, p - 1);
//                //Recursive calling for right segment
//                QuickSort(arrS, p + 1, end);
//            }
//
//        }
//        
//    };
//    Sort S;
//
//     S.QuickSort(arrS, 0, size-1);
//
//     //cout Each process in the output file 
//     for (int i = 0; i < size; i++)
//     {
//         for (int j = 0; j < size; j++)
//         {
//             if (prArr[j]->getCT() == arrS[i])
//             {
//                 //printing details of each process
//                 OutFile << "  " << prArr[j]->getTT() << "        " << prArr[j]->getPID() << "       " << prArr[j]->getAT() << "      " << prArr[j]->getCT() << "      ";
//
//                 //This part handles if a process have multiple IO_D's to print them
//                 int* arr= prArr[j]->getIO_D();
//                 int zz=0;
//                 while (arr[zz] > 0)
//                 {
//                     OutFile << arr[zz++] << " ";
//                 }
//                 arr = NULL;
//
//                 //printing details of each process
//                 OutFile << "           " << prArr[j]->getWT() << "      " << prArr[j]->getRT() << "       " << prArr[j]->getTRT() << endl;
//             }
//         }
//     }
//
//     //delete sorted array of TT 
//         delete[]arrS;
//         arrS = NULL;
//
//         OutFile << endl << endl <<"Processes: " << size << endl;
//
//         int sumRT = 0;
//         int sumWT = 0;
//         int sumTRT = 0;
//
//         for (int i = 0; i < size; i++)
//         {
//             sumWT = sumWT + prArr[i]->getWT();
//             sumRT = sumRT + prArr[i]->getRT();
//             sumTRT = sumTRT + prArr[i]->getTRT();
//         }
//
//         OutFile << "Avg WT = " << sumWT / size << "     " << "Avg RT = " << sumRT / size << "     " << "Avg TRT = " << sumTRT / size << endl;
//
//}
