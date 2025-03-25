#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    long long start;
    long long end;
    long long sum;
} ThreadData;

void *partial_sum(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    data->sum = 0;
    for (long long i = data->start; i <= data->end; i++) {
        data->sum += i;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <numThreads> <n>\n", argv[0]);
        return 1;
    }

    int numThreads = atoi(argv[1]);
    long long n = atoll(argv[2]);

    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];

    long long chunkSize = n / numThreads;
    long long sum = 0;

    clock_t start = clock();

    for (int i = 0; i < numThreads; i++) {
        threadData[i].start = i * chunkSize + 1;
        threadData[i].end = (i == numThreads - 1) ? n : (i + 1) * chunkSize;
        pthread_create(&threads[i], NULL, partial_sum, &threadData[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
        sum += threadData[i].sum;
    }

    clock_t end = clock();

    printf("Sum (Multi-Thread) = %lld\n", sum);
    printf("Time taken (Multi-Thread): %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
