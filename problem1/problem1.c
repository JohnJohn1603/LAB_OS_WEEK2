#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <string.h>

#define MAX_MOVIES 1682

typedef struct {
    int sharedMemRating[MAX_MOVIES + 1];
    int shareMemRateCnt[MAX_MOVIES + 1];
} sharedData;

int sem_id;

void lock() {
    struct sembuf sem_op = {0, -1, 0};
    semop(sem_id, &sem_op, 1);
}

void unlock() {
    struct sembuf sem_op = {0, 1, 0};
    semop(sem_id, &sem_op, 1);
}

void fileReadFunc(const char *filename, sharedData *shared_data) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File opening error");
        exit(1);
    }
    
    int userID, movieID, rating;
    unsigned long timestamp;
    while (fscanf(file, "%d\t%d\t%d\t%lu", &userID, &movieID, &rating, &timestamp) == 4) {
        lock();
        shared_data->sharedMemRating[movieID] += rating;
        shared_data->shareMemRateCnt[movieID] += 1;
        unlock();
    }
    
    fclose(file);
    exit(0);
}

int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(sharedData), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Shared memory creation error");
        exit(1);
    }
    
    sharedData *shared_data = (sharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (sharedData *)-1) {
        perror("Shared memory attach error");
        exit(1);
    }
    
    memset(shared_data, 0, sizeof(sharedData));

    sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    semctl(sem_id, 0, SETVAL, 1); 

    int pid1 = fork();
    if (pid1 == 0) {
        fileReadFunc("movie-100k_1.txt", shared_data);
    }

    int pid2 = fork();
    if (pid2 == 0) {
        fileReadFunc("movie-100k_2.txt", shared_data);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    for (int i = 1; i <= MAX_MOVIES; i++) {
        if (shared_data->shareMemRateCnt[i] > 0) {
            double avgRating = (double)shared_data->sharedMemRating[i] / shared_data->shareMemRateCnt[i];
            printf("Movie ID: %d \tAverage ratings: %f \n", i, avgRating);
        }
    }

    shmdt(shared_data);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);

    return 0;
}
