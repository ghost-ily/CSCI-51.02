#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

using namespace std;

const int MAX_FRAME_SIZE = 20000;
bool keep_running = true;

struct SharedVideoBuffer {
    int currentFrameNum;
    int totalFrames;
    char frame[MAX_FRAME_SIZE];
};

void *listener(void *ptr) {
    int input = cin.get();
    if (input == 10) {
        keep_running = false;
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <ASCII video file> <FPS>" << endl;
        return 1;
    }

    string file = argv[1];
    int fps = stoi(argv[2]);
    float sleepTime = (1000.0f / fps) * 1000; // microseconds

    ifstream inputFile(file);
    if (!inputFile.is_open()) {
        cerr << "Error: could not open file: " << file << endl;
        return 1;
    }

    // Create thread to listen for Enter key
    pthread_t thread;
    pthread_create(&thread, NULL, listener, NULL);

    // --- Semaphore setup ---
    key_t semKey = 1111;
    int semID = semget(semKey, 2, IPC_CREAT | 0666);
    if (semID == -1) {
        perror("semget error");
        return 1;
    }

    // sem 0 = empty slots: starts at 1 (buffer is empty, producer may write)
    // sem 1 = full slots:  starts at 0 (nothing to read yet)
    semctl(semID, 0, SETVAL, 1);
    semctl(semID, 1, SETVAL, 0);

    // --- Shared memory setup ---
    key_t shmKey = 2222;
    int shmSize = sizeof(SharedVideoBuffer);
    int shmID = shmget(shmKey, shmSize, IPC_CREAT | 0666);
    if (shmID == -1) {
        perror("shmget error");
        return 1;
    }

    SharedVideoBuffer* vidBuffer = (SharedVideoBuffer*)shmat(shmID, NULL, 0);
    if (vidBuffer == (SharedVideoBuffer*)-1) {
        perror("shmat failed");
        return 1;
    }

    vidBuffer->currentFrameNum = 0;
    vidBuffer->totalFrames     = 0;
    vidBuffer->frame[0]        = '\0';

    string currentLine;
    string pendingLine = ""; // holds the ESC+c line that ended the previous frame

    while (keep_running) {

        // Wait for an empty slot before writing
        struct sembuf wait_empty;
        wait_empty.sem_num = 0;
        wait_empty.sem_op  = -1;  // decrement: claim the empty slot
        wait_empty.sem_flg = SEM_UNDO;

        if (semop(semID, &wait_empty, 1) == -1) {
            perror("semop wait_empty error");
            break;
        }

        // --- Build one full frame into the shared buffer ---
        vidBuffer->frame[0] = '\0';
        bool gotFrame = false;

        // If the previous iteration left an ESC+c line, start this frame with it
        if (!pendingLine.empty()) {
            pendingLine += '\n';
            strncat(vidBuffer->frame, pendingLine.c_str(), MAX_FRAME_SIZE - 1);
            pendingLine = "";
        }

        // Read line by line; ESC+c (\033c) at the start of a line is the
        // frame separator (terminal reset sequence)
        while (getline(inputFile, currentLine)) {

            bool isFrameStart = currentLine.size() >= 2
                                && (unsigned char)currentLine[0] == 0x1B
                                && currentLine[1] == 'c';

            if (isFrameStart && vidBuffer->frame[0] != '\0') {
                // Current frame is complete; save this line for the next frame
                pendingLine = currentLine;
                gotFrame = true;
                break;
            }

            // Append line + newline into the frame buffer, guarding against overflow
            currentLine += '\n';
            size_t remaining = MAX_FRAME_SIZE - strlen(vidBuffer->frame) - 1;
            if (remaining > 0) {
                strncat(vidBuffer->frame, currentLine.c_str(), remaining);
            }
        }

        // Loop the file when EOF is reached
        if (inputFile.eof()) {
            inputFile.clear();
            inputFile.seekg(0);
        }

        if (gotFrame) {
            vidBuffer->currentFrameNum++;
            cout << "Producer: Wrote frame " << vidBuffer->currentFrameNum << endl;

            // Signal that a full slot is available for the consumer
            struct sembuf signal_full;
            signal_full.sem_num = 1;
            signal_full.sem_op  = 1;  // increment: mark slot as full
            signal_full.sem_flg = SEM_UNDO;

            if (semop(semID, &signal_full, 1) == -1) {
                perror("semop signal_full error");
                break;
            }
        } else {
            // No frame was built — give the empty slot back so we don't deadlock
            struct sembuf give_back;
            give_back.sem_num = 0;
            give_back.sem_op  = 1;
            give_back.sem_flg = SEM_UNDO;

            if (semop(semID, &give_back, 1) == -1) {
                perror("semop give_back error");
                break;
            }
        }

        usleep(sleepTime);
    }

    // Cleanup
    shmdt(vidBuffer);
    shmctl(shmID, IPC_RMID, NULL);
    semctl(semID, 0, IPC_RMID);

    inputFile.close();
    return 0;
}
