#include "logger.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

void *log_thread_func(void *arg) {
    int thread_id = *(int *)arg;

    for (int i = 0; i < 10; i++) {
        LOG(DBG, NULL, "Thread %d - Message %d", thread_id, i);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, log_thread_func, &thread_ids[i]) != 0) {
            LOG(ERR, NULL, "Failed to create thread %d", i + 1);
            return EXIT_FAILURE;
        }
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    logger_cleanup();
    return 0;
}
