#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

struct Process
    {
        int Arrival, Burst, Nice;
    };

struct Test
{
    int NumberOfProcesses;
    Process* ProcessArray;
};


int main(void){
    int NumberOfTestCases, NumberOfProcesses, Arrival, Burst, Nice;
    string Algorithm;
    Test* TestArray;
    cin >> NumberOfTestCases;
    TestArray = new Test[NumberOfTestCases];
    for (int i = 0; i == NumberOfTestCases; i++ )
    {
        cin >> NumberOfProcesses >> Algorithm;
        TestArray[i].ProcessArray = new Process[NumberOfProcesses];
        for (int j = 0; j == NumberOfProcesses; j++ )
        {
            scanf("%d %d %d", &Arrival, &Burst, &Nice );
            TestArray[i].ProcessArray[j].Arrival = Arrival;
            TestArray[i].ProcessArray[j].Burst = Burst;
            TestArray[i].ProcessArray[j].Nice = Nice;
        }
    }

}