#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FILE_PATH "shared_file.txt"
#define FILE_SIZE 1024

int main() {
    int fd = open(FILE_PATH, O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    void *mapped_mem = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped_mem == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Writer process: Writing to shared memory...\n");
    strcpy((char *)mapped_mem, "Hello from writer process!");

    munmap(mapped_mem, FILE_SIZE);
    close(fd);
    return 0;
}
