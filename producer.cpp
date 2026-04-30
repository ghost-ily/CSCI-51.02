#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <string.h>

using namespace std;

const int MAX_FRAME_SIZE = 20000; // Maximum size of a video frame
bool keep_running = true;

struct SharedVideoBuffer {
    int currentFrameNum;
    int totalFrames;
    char frame[MAX_FRAME_SIZE]; // Buffer to hold the video frame
};

// If input is 10 (ENTER), set keep_running to false
void *listener(void *ptr) {
    int input = cin.get();
    if (input == 10) {
        keep_running = false;
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <ASCII video file> <Video FPS>" << endl;
        return 1;
    }
    
    string file = argv[1];
    int fps = stoi(argv[2]);
    float sleepTime = (1000 / fps) * 1000; // Convert FPS to microseconds for usleep
    ifstream inputFile(file);

    // Create thread to take "ENTER" input
    pthread_t thread;
    pthread_create(&thread, NULL, listener, NULL);

    // Struct initialization
    SharedVideoBuffer* vidBuffer = new SharedVideoBuffer;
    vidBuffer->frame[0] = '\0';

    // Semaphore initialization
    int semID;
    key_t semKey = 1111;
    int semFlag = IPC_CREAT | 0666;
    int nSems = 2;

    semID = semget(semKey, nSems, semFlag);
    if (semID == -1) {
        perror("semget error");
        exit(1);
    }

    // Set semaphore operations to 4
    int semOps = 4;

    // Semaphore accessing
    struct sembuf sem[semOps];

    // Shared memory initialization
    key_t shmKey = 2222;
    int shmSize = sizeof(SharedVideoBuffer); // Size of shared memory segment
    int shmFlag = IPC_CREAT | 0666;
    int shmID = shmget(shmKey, shmSize, shmFlag);
    int opResult;

    while(keep_running) {
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

        // Semaphore 1: Producer will run exclusively (Lock mutex)
        sem[3].sem_num = 1;
        sem[3].sem_op = 1;
        sem[3].sem_flg = SEM_UNDO;

        opResult = semop(semID, sem, 4);
        char* sharedMem = (char*)shmat( shmID, NULL, 0 );

        if (opResult != 1) {
            // Read ASCII video file and write
            // frame to shared memory
            if(((int*)sharedMem) == (int*)-1) {
                perror( "shmop: shmat failed" );
                exit(1);
            }
            else {
                const char* line_buffer;
                string currentLine;


                if (inputFile.is_open()) {
                    // Continuously read each line from
                    // input file, then ignore the first
                    // instance of the ASCII ESC character
                    while (getline(inputFile, currentLine, char(27))) {
                         
                        // If the line is empty (and the frame started with
                        // the ESC characters), manually add it to the 
                        // frame buffer
                        if (currentLine.empty() && vidBuffer->frame[0] == '\0') {
                            currentLine = currentLine + char(27);
                            cout << "Started new frame" << endl;
                            continue;
                        }

                        // If the line reaches the start of the
                        // other frame (the starting ESC character),
                        // place frame into shared memory and
                        // signal out that current frame is full
                        if (currentLine.empty()) {
                            // Release semaphores after this
                            strcpy(sharedMem, vidBuffer->frame);
                            cout << "Finished frame" << endl;
                        }
                        else {
                            // else, copy current line into vidBuffer
                            strcpy(vidBuffer->frame, currentLine.c_str());
                            continue;
                        }

                        // Check if end of file is reached, then
                        // clears the eof flag
                        if (inputFile.eof()) {
                            inputFile.clear();
                            inputFile.seekg(0);
                        }

                        // Semaphore releasing

                        // Set number of operations to 2
                        semOps = 1;

                        // Semaphore 1: Producer releases semaphore (Unlock mutex)
                        sem[0].sem_num = 1;
                        sem[0].sem_op = -1;
                        sem[0].sem_flg = SEM_UNDO;

                        opResult = semop(semID, sem, semOps);
                        if( opResult == -1 ){
                            perror( "semop (decrement)" );
                        }

                        usleep(sleepTime);
                    }
                }
            }
        }
    }

    return 0;
}