#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <thread>

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
    float sleepTime = 1000 / fps;
    ifstream inputFile(file);

    // Create thread to take "ENTER" input
    pthread_t thread;
    pthread_create(&thread, NULL, listener, NULL);

    // Struct initialization
    SharedVideoBuffer* vidBuffer = new SharedVideoBuffer;

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

    // Shared memory initialization
    key_t shmKey = 2222;
    int shmSize = sizeof(SharedVideoBuffer); // Size of shared memory segment
    int shmFlag = IPC_CREAT | 0666;
    int shmID = shmget(shmKey, shmSize, shmFlag);

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

        // Semaphore 1: Producer will run exclusively
        sem[3].sem_num = 1;
        sem[3].sem_op = 1;
        sem[3].sem_flg = SEM_UNDO;

        int opResult = semop(semID, sem, 4);
        char* sharedMem = (char*)shmat( shmID, NULL, 0 );

        if (opResult != 1) {
            // Read ASCII video file and write
            // frame to shared memory
            if(((int*)sharedMem) == (int*)-1) {
                perror( "shmop: shmat failed" );
                exit(1)
            }
            else {
                const char* line_buffer;
                string currentLine;

                if (inputFile.is_open()) {
                    // Continuously read each line from
                    // input file, then ignore the first
                    // instance of the ASCII ESC character
                    while (getline(inputFile, currentLine, char(27))) {

                    }
                }
                
                strcpy( sharedMem, line_buffer );
                char buffer2[50];
                
                strcpy( buffer2, sharedMem );
                printf( "%s\n", buffer2 );
                char buffer3[50];
                
                memcpy( buffer3, sharedMem, 3 );
                buffer3[3] = '\0';
                printf( "%s\n", buffer3 );
            }
        }
    }

    return 0;
}