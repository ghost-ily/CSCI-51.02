#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

struct Process
{
    int Arrival;         // Time when the process arrives in the system
    int Burst;           // Total CPU time required by the process
    int Nice;            // Priority value 
    int Remaining;       // Remaining CPU time 
    int CompletionTime;  // Time when the process finishes execution
    int StartTime;       // Time when the process first starts running
    bool Started;        // True if process has started at least once 
};

struct Test
{
    int NumberOfProcesses; 
    string testAlgorithm; 
    Process* ProcessArray;
    int Quantum;
};

//This function calculates and prints all performance statistics after a scheduling algorithm finishes running
void printStats(Process readyQueue[], int processIndex[], int n, int totalTime, int burstTime)
{
    cout << "Total time elapsed: " << totalTime << "ns" << endl;
    cout << "Total CPU burst time: " << burstTime << "ns" << endl;

    double cpuUtil = ((double)burstTime / totalTime) * 100;
    cout << "CPU Utilization: " << (int)(cpuUtil + 0.5) << "%" << endl;

    double throughput = (double)n / totalTime;
    cout << "Throughput: " << throughput << " processes/ns" << endl;

    double totalWaiting = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;

    cout << "Waiting times:" << endl;
    for (int p = 1; p <= n; p++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (processIndex[j] == p)
            {
                int turnaround = readyQueue[j].CompletionTime - readyQueue[j].Arrival;
                int waiting = turnaround - readyQueue[j].Burst;
                int response = readyQueue[j].StartTime - readyQueue[j].Arrival;

                totalWaiting += waiting;
                totalTurnaround += turnaround;
                totalResponse += response;

                cout << " Process " << p << ": " << waiting << "ns" << endl;
                break;
            }
        }
    }

    cout << "Average waiting time: " << totalWaiting / n << "ns" << endl;
    cout << "Turnaround times:" << endl;

    for (int p = 1; p <= n; p++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (processIndex[j] == p)
            {
                int turnaround = readyQueue[j].CompletionTime - readyQueue[j].Arrival;
                cout << " Process " << p << ": " << turnaround << "ns" << endl;
                break;
            }
        }
    }

    cout << "Average turnaround time: " << totalTurnaround / n << "ns" << endl;
    cout << "Response times:" << endl;

    for (int p = 1; p <= n; p++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (processIndex[j] == p)
            {
                int response = readyQueue[j].StartTime - readyQueue[j].Arrival;
                cout << " Process " << p << ": " << response << "ns" << endl;
                break;
            }
        }
    }
    cout << "Average response time: "  << totalResponse / n << "ns" << endl;
}

int main(void){
    /*
        Initializing test cases, algorithms, and processes
    */
    int NumberOfTestCases, NumberOfProcesses, Arrival, Burst, Nice;
    string Algorithm;
    Test* TestArray;
    cin >> NumberOfTestCases;
    TestArray = new Test[NumberOfTestCases];

    for (int i = 0; i < NumberOfTestCases; i++)
    {
        cin >> NumberOfProcesses >> Algorithm;
        TestArray[i].ProcessArray = new Process[NumberOfProcesses];
        TestArray[i].testAlgorithm = Algorithm;
        TestArray[i].NumberOfProcesses = NumberOfProcesses;
        if (Algorithm == "RR")
            cin >> TestArray[i].Quantum;
            
        for (int j = 0; j < NumberOfProcesses; j++)
        {
            scanf("%d %d %d", &Arrival, &Burst, &Nice);
            TestArray[i].ProcessArray[j].Arrival = Arrival;
            TestArray[i].ProcessArray[j].Burst = Burst;
            TestArray[i].ProcessArray[j].Nice = Nice;
            TestArray[i].ProcessArray[j].Remaining = Burst;
            TestArray[i].ProcessArray[j].CompletionTime = 0;
            TestArray[i].ProcessArray[j].StartTime = -1;
            TestArray[i].ProcessArray[j].Started = false;
        }
    }

    /*
        Executing test cases and algorithms
    */
    for (int i = 0; i < NumberOfTestCases; i++)
    {
        int totalTime, burstTime, idleTime = 0;
        Test currentTest = TestArray[i];
        string currentAlgo = currentTest.testAlgorithm;
        Process* readyQueue = currentTest.ProcessArray;
        int processIndex[currentTest.NumberOfProcesses];

        for (int j = 0; j < currentTest.NumberOfProcesses; j++){
            processIndex[j] = j + 1;
        }

        /*
            Arranging processes according to arrival time for
            smoother internal execution
        */
        for (int j = 0; j < currentTest.NumberOfProcesses - 1; j++)
        {
            int minIndex = j;
            Process minProcess = readyQueue[j];

            for (int k = j + 1; k < currentTest.NumberOfProcesses; k++)
            {
                if (readyQueue[k].Arrival < minProcess.Arrival)
                {
                    minIndex = k;
                    minProcess = readyQueue[k];
                }
            }
            Process temp = readyQueue[j];
            readyQueue[j] = readyQueue[minIndex];
            readyQueue[minIndex] = temp;

            int tempIndex = processIndex[j];
            processIndex[j] = processIndex[minIndex];
            processIndex[minIndex] = tempIndex;
        }

        //====== FCFS =====
        if (currentAlgo == "FCFS")
        {
            cout << i + 1 << " " << currentAlgo << endl;
            totalTime = 0;
            burstTime = 0;
            idleTime = 0;

            for (int j = 0; j < currentTest.NumberOfProcesses; j++)
            {
                Process runningProcess = readyQueue[j];
                // If the CPU is idle and the next process arrives later than current time then we fast-forward time to its arrival.
                if (totalTime < runningProcess.Arrival)
                {
                    idleTime += (runningProcess.Arrival - totalTime); // Add idle time 
                    totalTime = runningProcess.Arrival; // Move simulation time forward to process arrival
                }

                //Record the time when process starts.
                readyQueue[j].StartTime = totalTime;

                cout << totalTime << " " << processIndex[j];

                totalTime += runningProcess.Burst;   // Advance time
                burstTime += runningProcess.Burst;   // Add to total CPU usage

                // Record when process finishes.
                readyQueue[j].CompletionTime = totalTime;
                cout << " " << runningProcess.Burst << "X" << endl;
            }
            printStats(readyQueue, processIndex, currentTest.NumberOfProcesses, totalTime, burstTime);
        }
        

        //===========SJF===========
        if (currentAlgo == "SJF")
        {
            cout << i + 1 << " " << currentAlgo << endl;

            totalTime = 0;
            burstTime = 0;
            idleTime = 0;

            int completed = 0;
            bool finished[currentTest.NumberOfProcesses]; // tracks whether a process has already been executed

            for (int j = 0; j < currentTest.NumberOfProcesses; j++)
                finished[j] = false;

            while (completed < currentTest.NumberOfProcesses)
            {
                int shortestIndex = -1;  // index of shortest job found
                int shortestBurst = 1000000; //large number (acts like infinity)

                // Find shortest job that has arrived
                for (int j = 0; j < currentTest.NumberOfProcesses; j++)
                {
                    if (!finished[j] &&
                        readyQueue[j].Arrival <= totalTime &&
                        readyQueue[j].Burst < shortestBurst)
                    {
                        shortestBurst = readyQueue[j].Burst;
                        shortestIndex = j;
                    }
                }

                // If no process has arrived yet then CPU is idle
                if (shortestIndex == -1)
                {
                    totalTime++;
                    idleTime++;
                    continue;
                }

                // Record start time
                readyQueue[shortestIndex].StartTime = totalTime;

                cout << totalTime << " "
                    << processIndex[shortestIndex];

                totalTime += readyQueue[shortestIndex].Burst;
                burstTime += readyQueue[shortestIndex].Burst;

                // Record completion time
                readyQueue[shortestIndex].CompletionTime = totalTime;

                cout << " "
                    << readyQueue[shortestIndex].Burst
                    << "X" << endl;

                finished[shortestIndex] = true;
                completed++;
            }
            printStats(readyQueue, processIndex, currentTest.NumberOfProcesses, totalTime, burstTime);
        }
        
        //=======SRTF=============
        if (currentAlgo == "SRTF")
        {
            cout << i + 1 << " " << currentAlgo << endl;
            totalTime = 0;
            burstTime = 0;
            idleTime = 0;

            int completed = 0;
            int lastProcess = -1; // Stores the index of the process that ran in the previous time unit
            int blockStart = 0;   // track start of current block

            while (completed < currentTest.NumberOfProcesses)
            {
                int shortestIndex = -1;
                int shortestRemaining = 1000000;

                // Find process with smallest remaining time
                for (int j = 0; j < currentTest.NumberOfProcesses; j++)
                {
                    if (readyQueue[j].Arrival <= totalTime &&
                        readyQueue[j].Remaining > 0)
                    {
                        if (shortestIndex == -1 ||
                            readyQueue[j].Remaining < shortestRemaining)
                        {
                            shortestRemaining = readyQueue[j].Remaining;
                            shortestIndex = j;
                        }
                    }
                }

                // CPU idle
                if (shortestIndex == -1)
                {
                    totalTime++;
                    idleTime++;
                    continue;
                }


                // checks if current selected process is different from the previous one (context switch detection)
                if (lastProcess != shortestIndex)
                {
                    if (lastProcess != -1)
                    {
                    cout << blockStart << " " << processIndex[lastProcess] << " " << totalTime - blockStart << endl;
                    }
                    blockStart = totalTime;
                    lastProcess = shortestIndex;
                }

                // Record start time
                if (!readyQueue[shortestIndex].Started)
                {
                    readyQueue[shortestIndex].StartTime = totalTime;
                    readyQueue[shortestIndex].Started = true;
                }
                readyQueue[shortestIndex].Remaining--;
                burstTime++;
                totalTime++;

            // If process finishes then print block with X
                if (readyQueue[shortestIndex].Remaining == 0)
                {
                    readyQueue[shortestIndex].CompletionTime = totalTime;
                    cout << blockStart << " " << processIndex[shortestIndex] << " " << totalTime - blockStart << "X" << endl;
                    completed++;
                    lastProcess = -1;
                }
            }

            printStats(readyQueue, processIndex, currentTest.NumberOfProcesses, totalTime, burstTime);
        }

        //===== RR =====
        if (currentAlgo == "RR")
        {
            cout << i + 1 << " " << currentAlgo << endl;

            totalTime = 0;
            burstTime = 0;
            idleTime = 0;

            int finishedProcesses = 0;
            int q = currentTest.Quantum;
            int* newProcessIndex = new int[currentTest.NumberOfProcesses];
            Process* arrivedProcesses = new Process[currentTest.NumberOfProcesses];

            for (int j = 0; j < currentTest.NumberOfProcesses; j++)
            {
                arrivedProcesses[currentTest.NumberOfProcesses - j] = readyQueue[j];
                newProcessIndex[currentTest.NumberOfProcesses - j] = processIndex[j];
            }
            
            while (finishedProcesses < currentTest.NumberOfProcesses)
            {

                for (int j = 0; j <= currentTest.NumberOfProcesses; j++)
                {
                    if (totalTime >= arrivedProcesses[j].Arrival &&
                        arrivedProcesses[j].Remaining > 0 &&
                        arrivedProcesses[j].Burst > 0)
                    {
                        cout << totalTime << " ";

                        int execTime = arrivedProcesses[j].Remaining;
                        if (q < execTime)
                        {
                            execTime = q;
                        }

                        arrivedProcesses[j].Remaining -= execTime;
                        if (arrivedProcesses[j].Started == false)
                        {
                            arrivedProcesses[j].Started = true;
                            readyQueue[currentTest.NumberOfProcesses-j].StartTime = totalTime;
                        }
                        totalTime += execTime;
                        burstTime += execTime;

                        cout << newProcessIndex[j] << " ";

                        if (arrivedProcesses[j].Remaining == 0)
                        {
                            readyQueue[currentTest.NumberOfProcesses-j].CompletionTime = totalTime;
                            cout << execTime;
                            cout << "X" << endl;
                            finishedProcesses++;
                        }
                        else
                        {
                            cout << execTime << endl;
                        }
                    }
                }
            }

            printStats(readyQueue, processIndex, currentTest.NumberOfProcesses, totalTime, burstTime);
        }
    }
}