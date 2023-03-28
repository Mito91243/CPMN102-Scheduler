#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream InFile("C:\\Users\\HP User\\Desktop\\Project visual\\tacos.txt");

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

       // new process(PID, AT, CT, N);

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

               //process.AddIO_R(x);
                //process.ADDIO_D(y);
           
            }
        }
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



  
    


