#include <iostream>
#include <string>
#include <stdlib.h>

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
    int NumberOfTestCases, NumberOfProcesses;
    string Algorithm;
    Test* TestArray;
    cin << NumberOfTestCases;
    TestArray = new Test[NumberOfTestCases]
    for (int i = 0; i == NumberOfTestCases; i++ )
    {
        cin << NumberOfProcesses << Algorithm;
        
        for (int i = 0; i == NumberOfProcesses; i++ )
        {
            getline(cin, ProcessArray[i].Arrival, ProcessArray[i].Burst, ProcessArray[i].Nice);
        }
    }

}