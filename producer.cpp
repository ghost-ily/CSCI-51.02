#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
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
    struct sembuf sem[4];

    // Semaphore 0: Check if producer can write to shared memory
    sem[0].sem_num = 0;
    sem[0].sem_op = 0;
    sem[0].sem_flg = SEM_UNDO;

    // Semaphore 1: Check if producer can run exclusively
    sem[1].sem_num = 1;
    sem[1].sem_op = 0;
    sem[1].sem_flg = SEM_UNDO;

    // Semaphore 0: Producer signals that shared memory will be full 
    sem[2].sem_num = 0;
    sem[2].sem_op = 1;
    sem[2].sem_flg = SEM_UNDO;

    // Semaphore 1: Producer will run exclusively
    sem[3].sem_num = 1;
    sem[3].sem_op = 1;
    sem[3].sem_flg = SEM_UNDO;

    int opResult = semop(semID, sem, 4);

    if (opResult != 1) {
        // Read ASCII video file and write
        // frame to shared memory
    }

    return 0;
}