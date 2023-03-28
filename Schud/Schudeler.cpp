#include <iostream>
#include <fstream>
#include "Schudeler.h"
#include "process.h"

using namespace std;

void Schudeler::LoadInput()
{
    //Open the File .txt
    ifstream InFile("C:\\Users\\HP User\\Desktop\\Project visual\\tacos.txt");

    //Checks if the File is opened
    if (InFile.fail())
    {
        cout << "Error Opening the file: " << endl;
        exit(1);
    }

    //Read number of each Processor (First Line)
    int FCFS, SJF, RR;
    InFile >> FCFS >> SJF >> RR;

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
        InFile >> PID >> AT >> CT >> N;

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
        //Wait to see which process is empty and enqueue the process to it
        //And then loop again with same idea for ProcessNum
        //Get the empty processor
        // processor->enqueue(p)
    }

    //Read the SIGKILL and the PID
    int PIDD, SIGKILL;
    while (InFile.peek() != EOF)
    {
        InFile >> PIDD >> SIGKILL;
        cout << "PID SIG: " << PIDD << endl << "SigKILL: " << SIGKILL << endl;
    }

    InFile.close();

}
void Schudeler::Run()
{
    LoadInput();
    // More Functions to be added

}
