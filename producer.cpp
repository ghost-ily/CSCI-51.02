#include <fstream>
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <ASCII video file> <Video FPS>" << endl;
        return 1;
    }
    
    string file = argv[1];
    int fps = stoi(argv[2]);

    // Semaphore initialization
    int semID;
    key_t semKey = 1111;
    int semFlag = IPC_CREAT | 0666;
    int nSems = 3;

    semID = semget(semKey, nSems, semFlag);
    if (semID == -1) {
        perror("semget error");
        exit(1);
    }

    // Semaphore accessing
    struct sembuf sem[2];

    // Semaphore 0: Signifies when producer can write
    sem[0].sem_num = 0;
    sem[0].sem_op = -1;
    sem[0].sem_flg = SEM_UNDO;

    // Semaphore 1: Mutex semaphore
    sem[1].sem_num = 1;
    sem[1].sem_op = -1;
    sem[1].sem_flg = SEM_UNDO;


    return 0;
}