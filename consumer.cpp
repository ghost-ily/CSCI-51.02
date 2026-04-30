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
}