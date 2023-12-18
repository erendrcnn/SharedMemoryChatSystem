#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>

// Prepared By Eren Durucan

#define SHRDM_LENGTH 1024           // Size of shared memory
#define SHRDM_KEY_NUM 12345         // Key for shared memory
#define MAX_USERS_NUM 99999         // Maximum number of users
#define MESSAGE_SIGNAL SIGUSR1      // Signal for receiving a message
#define UPDATE_SIGNAL SIGINT        // Signal for program termination

// Structure for shared memory
struct SharedMemoryStr {
    int counter;
    int userDB[MAX_USERS_NUM];
    char sender[SHRDM_LENGTH];
    char msg[SHRDM_LENGTH];
};

// Pointer to shared memory
struct SharedMemoryStr *shared_memory_ptr;

// Username
char username[SHRDM_LENGTH];

// Functions
void receive_message(int signal);
void remove_storage(int signum);
void update_storage(int signum);

static int shmid;

// Main Driver
int main(int argc, char *argv[]) {
    if (argc != 2) {
        // Usage: fprintf(stderr, "Usage: %s <username>\n", argv[0]);
        exit(1);
    }

    strcpy(username, argv[1]);  // Username taken from argument

    int curr_pid = getpid();    // Current user PID

    // Create shared memory
    shmid = shmget(SHRDM_KEY_NUM, sizeof(struct SharedMemoryStr), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory segment
    shared_memory_ptr = (struct SharedMemoryStr *)shmat(shmid, NULL, 0);
    if (shared_memory_ptr == (struct SharedMemoryStr *)(-1)) {
        perror("shmat");
        exit(1);
    }

    if (shared_memory_ptr->counter > 0 && shared_memory_ptr->counter < MAX_USERS_NUM) {
        // Add other users
        shared_memory_ptr->userDB[shared_memory_ptr->counter] = curr_pid;
        shared_memory_ptr->counter++;
    } else {
        // Add the first user
        shared_memory_ptr->counter = 0;
        shared_memory_ptr->userDB[shared_memory_ptr->counter] = curr_pid;
        shared_memory_ptr->counter++;
    }

    // Signal handling for receiving messages
    signal(MESSAGE_SIGNAL, *receive_message);
    // Signal handling for program termination
    signal(UPDATE_SIGNAL, *remove_storage);

    printf("-- %s --\n", username);

    while (1) {
        char message[SHRDM_LENGTH];
        fgets(message, SHRDM_LENGTH, stdin);

        // Remove newline character
        message[strlen(message) - 1] = '\0';

        // Write user message to shared memory
        strcpy(shared_memory_ptr->sender, username);
        strcpy(shared_memory_ptr->msg, message);
        
        // Send message to all users except self
        for (int i = 0; i < shared_memory_ptr->counter; i++) {
            if (shared_memory_ptr->userDB[i] != curr_pid) {
                kill(shared_memory_ptr->userDB[i], MESSAGE_SIGNAL);
            }
        }
    }

    return 0;
}

// Message receiving function
void receive_message(int signal) {
    if (signal == MESSAGE_SIGNAL) {
        printf("[%s]: %s\n", shared_memory_ptr->sender, shared_memory_ptr->msg);
        fflush(stdout);
    }
}

// Reset when a user leaves
void remove_storage(int signum)
{
    shared_memory_ptr->counter = 0;
    shmdt((void*)shared_memory_ptr);
    shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}

// This function will not be used.
// Update shared memory when a user exits
void update_storage(int signum) {
    int curr_pid = getpid();
    int curr_index = -1;

    // Remove user from shared memory
    for (int i = 0; i < shared_memory_ptr->counter; i++) {
        if (shared_memory_ptr->userDB[i] == curr_pid) {
            shared_memory_ptr->userDB[i] = -1;
            curr_index = i;
        }
    }

    // Shift elements to the left after the user
    for (int i = curr_index; i < shared_memory_ptr->counter - 1; i++) {
        shared_memory_ptr->userDB[i] = shared_memory_ptr->userDB[i + 1];
    }

    // Decrease user count
    shared_memory_ptr->counter--;

    //printf("User {%s} left the chat!\n", username);

    exit(0);
}