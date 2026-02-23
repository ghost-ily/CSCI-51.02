#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

struct Process
{
    int Arrival, Burst, Nice;
};

struct Test
{
    int NumberOfProcesses;
    string testAlgorithm;
    Process* ProcessArray;
};


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
        for (int j = 0; j < NumberOfProcesses; j++)
        {
            scanf("%d %d %d", &Arrival, &Burst, &Nice);
            TestArray[i].ProcessArray[j].Arrival = Arrival;
            TestArray[i].ProcessArray[j].Burst = Burst;
            TestArray[i].ProcessArray[j].Nice = Nice;
        }
    }

    /*
        Executing test cases and algorithms
    */
    for (int i = 0; i < NumberOfTestCases; i++)
    {
        int totalTime, burstTime = 0;
        Test currentTest = TestArray[i];
        string currentAlgo = currentTest.testAlgorithm;
        Process* readyQueue = currentTest.ProcessArray;
        int processIndex[currentTest.NumberOfProcesses];

        for (int j = 0; j < currentTest.NumberOfProcesses; j++){
            processIndex[j] = j;
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

        if (currentAlgo == "FCFS")
        {
            cout << to_string(currentTest.NumberOfProcesses) << " " << currentAlgo << endl;
            totalTime = 0;
            for (int j = 0; j < currentTest.NumberOfProcesses; j++)
            {
                Process runningProcess = readyQueue[j];
                
                if (totalTime < runningProcess.Arrival)
                {
                    totalTime = (runningProcess.Arrival);
                    cout << to_string(totalTime) << " " << to_string(processIndex[j] + 1);
                    totalTime += runningProcess.Burst;
                }
                else
                {
                    cout << to_string(totalTime) << " " << to_string(processIndex[j] + 1);
                    totalTime += runningProcess.Burst;
                }
                burstTime += runningProcess.Burst;
                cout << " " << to_string(readyQueue[j].Burst) << "X" << endl;
            }
        }

        if (currentAlgo == "SJF")
        {

        }
    }

    /*
        Deallocating memory
    
    free(TestArray);
    TestArray = NULL;
    return 0;
    */
}