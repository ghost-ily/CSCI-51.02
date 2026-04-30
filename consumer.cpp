#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

using namespace std;

const int MAX_FRAME_SIZE = 20000;
bool keep_running = true;

struct SharedVideoBuffer {
    int currentFrameNum;
    int totalFrames;
    char frame[MAX_FRAME_SIZE];
};

// Re-use your listener thread logic for the Enter key
void *listener(void *ptr) {
    cin.get();
    keep_running = false;
    return NULL;
}

int main(int argc, char* argv[]) {
    int fps = stoi(argv[1]);
    float sleepTime = (1000 / fps) * 1000; // Convert FPS to microseconds for usleep

    // Create thread to listen for Enter key
    pthread_t thread;
    pthread_create(&thread, NULL, listener, NULL);
    

    // Semaphore initialization
    int semID;
    key_t semKey = 1111;
    int semFlag = IPC_CREAT | 0666;
    int nSems = 2;

    semID = semget(semKey, nSems, semFlag);
    if (semID == -1) {
        perror("semget error");
    }

    int lastFrameSeen = 0;
    int totalSkipped = 0;

    while (keep_running) {
        // Check if producer can write to shared memory
        struct sembuf sem[2];
        sem[0].sem_num = 0;
        sem[0].sem_op = 0;
        sem[0].sem_flg = SEM_UNDO;

        // Check if producer can run exclusively
        sem[1].sem_num = 1;
        sem[1].sem_op = 0;
        sem[1].sem_flg = SEM_UNDO;

        int opResult = semop(semID, sem, 2);
        if (opResult == -1) {
            perror("semop error");
            exit(1);
        }

        // Access shared memory and read the current frame number
        key_t shmKey = 2222;
        int shmSize = sizeof(SharedVideoBuffer);
        int shmFlag = IPC_CREAT | 0666;
        int shmID = shmget(shmKey, shmSize, shmFlag);
        char* sharedMem = (char*)shmat(shmID, NULL, 0);

        if (sharedMem == (char*)-1) {
            perror("shmat failed");
            exit(1);
        }

        SharedVideoBuffer* vidBuffer = (SharedVideoBuffer*)sharedMem;
        
        // Check if the current frame number is greater than the last seen frame number
        if (vidBuffer->currentFrameNum > lastFrameSeen) {
            cout << "Producer: Current Frame Number: " << vidBuffer->currentFrameNum << endl;
            lastFrameSeen = vidBuffer->currentFrameNum;
            totalSkipped += (vidBuffer->currentFrameNum - lastFrameSeen - 1);
            cout << "Producer: Total Skipped Frames: " << totalSkipped << endl;
        }

        shmdt(sharedMem); // Detach from shared memory

        usleep(sleepTime); // Sleep for the calculated time based on FPS
    }
}