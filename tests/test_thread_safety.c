#include "../clog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _POSIX_THREADS
#include <pthread.h>
#define HAS_THREADS 1
#else
#define HAS_THREADS 0
#endif

#define TEST_ASSERT(condition, message)                                        \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr, "❌ ASSERTION FAILED: %s\n", message);                   \
      exit(EXIT_FAILURE);                                                      \
    } else {                                                                   \
      printf("✅ PASSED: %s\n", message);                                      \
    }                                                                          \
  } while (0)

#define TEST_START(name)                                                       \
  printf("\n🧪 Starting test: %s\n", name);                                    \
  printf("=====================================\n")

#define TEST_END(name)                                                         \
  printf("✅ Test completed: %s\n", name);                                     \
  printf("=====================================\n")

#if HAS_THREADS
typedef struct {
  int thread_id;
  int message_count;
} thread_data_t;

static void *thread_log_function(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;
  for (int i = 0; i < 1000; i++) {
    INFO("Thread %d message %d", data->thread_id, i);
  }
  data->message_count = 1000;
  return NULL;
}

extern void test_thread_safety(void) {
  TEST_START("Thread Safety");
  const int num_threads = 10;
  pthread_t threads[num_threads];
  thread_data_t thread_data[num_threads];

  printf("Starting %d threads...\n", num_threads);
  for (int i = 0; i < num_threads; i++) {
    thread_data[i].thread_id = i;
    thread_data[i].message_count = 0;
    TEST_ASSERT(pthread_create(&threads[i], NULL, thread_log_function,
                               &thread_data[i]) == 0,
                "Thread creation");
  }
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
    TEST_ASSERT(thread_data[i].message_count == 1000,
                "Thread logged all messages");
  }
  TEST_END("Thread Safety");
}
#else
void test_thread_safety(void) {
  printf("⚠️  Thread safety test skipped (pthread not available)\n");
}
#endif
