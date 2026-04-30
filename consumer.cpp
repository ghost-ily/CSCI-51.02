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

void *listener(void *ptr) {
    cin.get();
    keep_running = false;
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <FPS>" << endl;
        return 1;
    }

    int fps = stoi(argv[1]);
    float sleepTime = (1000.0f / fps) * 1000; // microseconds

    // Create thread to listen for Enter key
    pthread_t thread;
    pthread_create(&thread, NULL, listener, NULL);

    // --- Semaphore setup ---
    key_t semKey = 1111;
    int semID = semget(semKey, 2, 0666);
    if (semID == -1) {
        perror("semget error");
        return 1;
    }

    // --- Shared memory setup ---
    key_t shmKey = 2222;
    int shmSize = sizeof(SharedVideoBuffer);
    int shmID = shmget(shmKey, shmSize, 0666);
    if (shmID == -1) {
        perror("shmget error");
        return 1;
    }

    SharedVideoBuffer* vidBuffer = (SharedVideoBuffer*)shmat(shmID, NULL, 0);
    if (vidBuffer == (SharedVideoBuffer*)-1) {
        perror("shmat failed");
        return 1;
    }

    int lastFrameSeen = 0;
    int totalSkipped  = 0;

    while (keep_running) {

        // Wait for a full slot to be available
        struct sembuf wait_full;
        wait_full.sem_num = 1;
        wait_full.sem_op  = -1;  // decrement: claim the full slot
        wait_full.sem_flg = SEM_UNDO;

        if (semop(semID, &wait_full, 1) == -1) {
            perror("semop wait_full error");
            break;
        }

        // --- Read the frame ---
        if (vidBuffer->currentFrameNum > lastFrameSeen) {
            int skipped   = vidBuffer->currentFrameNum - lastFrameSeen - 1;
            totalSkipped += skipped;
            lastFrameSeen = vidBuffer->currentFrameNum;

            cout << "Consumer: Current Frame Number: " << lastFrameSeen << endl;
            cout << "Consumer: Total Skipped Frames: " << totalSkipped  << endl;
            cout << vidBuffer->frame;
            cout.flush();
        }

        // Signal that the slot is now empty again
        struct sembuf signal_empty;
        signal_empty.sem_num = 0;
        signal_empty.sem_op  = 1;  // increment: mark slot as empty
        signal_empty.sem_flg = SEM_UNDO;

        if (semop(semID, &signal_empty, 1) == -1) {
            perror("semop signal_empty error");
            break;
        }

        usleep(sleepTime);
    }

    // Cleanup
    shmdt(vidBuffer);

    return 0;
}
