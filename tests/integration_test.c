#include "../clog.h"
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#define NUM_THREADS 2
#define MESSAGES_PER_THREAD 10

#ifdef _WIN32
typedef DWORD(WINAPI *thread_func_t)(LPVOID);
#else
typedef void *(*thread_func_t)(void *);
#endif

void *thread_log_function(void *arg) {
  int thread_id = *(int *)arg;
  for (int i = 0; i < MESSAGES_PER_THREAD; i++) {
    char msg[256];
    snprintf(msg, sizeof(msg), "Thread %d message %d", thread_id, i);
    INFO("%s", msg);
  }
  return NULL;
}

extern void test_integration(void) {
  // Open log file
  FILE *file = fopen("integration_test.log", "w");
  if (!file) {
    fprintf(stderr, "Failed to open log file\n");
    exit(EXIT_FAILURE);
  }
  clog_set_output(file);

  // Set log level to TRACE
  clog_set_level(CLOG_TRACE);

  // Log messages at all levels
  TRACE("TRACE message");
  DEBUG("DEBUG message");
  INFO("INFO message");
  WARN("WARN message");
  ERROR("ERROR message");
  FATAL("FATAL message");

  // Toggle timestamps
  clog_set_show_timestamp(false);
  INFO("Message without timestamp");
  clog_set_show_timestamp(true);
  INFO("Message with timestamp");

  // Toggle location
  clog_set_show_location(false);
  INFO("Message without location");
  clog_set_show_location(true);
  INFO("Message with location");

  // Log Unicode message
  INFO("Unicode: café 😊");

  // Create threads
#ifdef _WIN32
  HANDLE threads[NUM_THREADS];
  int thread_ids[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i;
    threads[i] =
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_log_function,
                     &thread_ids[i], 0, NULL);
    if (!threads[i]) {
      fprintf(stderr, "Failed to create thread %d\n", i);
      exit(EXIT_FAILURE);
    }
  }
  WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);
  for (int i = 0; i < NUM_THREADS; i++) {
    CloseHandle(threads[i]);
  }
#else
  pthread_t threads[NUM_THREADS];
  int thread_ids[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i;
    if (pthread_create(&threads[i], NULL, thread_log_function,
                       &thread_ids[i]) != 0) {
      fprintf(stderr, "Failed to create thread %d\n", i);
      exit(EXIT_FAILURE);
    }
  }
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
#endif

  // Close log file
  fclose(file);

  // Verify log file
  file = fopen("integration_test.log", "r");
  if (!file) {
    fprintf(stderr, "Failed to reopen log file\n");
    exit(EXIT_FAILURE);
  }
  char buf[256];
  while (fgets(buf, sizeof(buf), file)) {
    printf("%s", buf);
  }
  fclose(file);
  remove("integration_test.log");
}
