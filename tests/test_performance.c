#include "../clog.h"
#include <stdio.h>
#include <time.h>

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

extern void test_performance(void) {
  TEST_START("Performance");
  const int message_count = 100000;
  clock_t start = clock();
  for (int i = 0; i < message_count; i++) {
    TRACE("Perf message %d", i);
  }
  clock_t end = clock();
  double elapsed_ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
  printf("Logged %d messages in %.2f ms\n", message_count, elapsed_ms);
  TEST_ASSERT(elapsed_ms < 10000, "Performance within timeout");
  TEST_END("Performance");
}
