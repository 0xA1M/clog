#include "../clog.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#define usleep(t) Sleep(t / 1000)
#else
#include <unistd.h>
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

static volatile sig_atomic_t signal_test_complete = 0;
static volatile sig_atomic_t signal_log_count = 0;

static void signal_handler(int sig) {
  (void)sig; // Suppress unused variable compiler warning
  signal_log_count++;
  clog_signal_log("Signal received!");
  if (signal_log_count >= 5)
    signal_test_complete = 1;
  signal(SIGINT, signal_handler);
}

extern void test_signal_safety(void) {
  TEST_START("Signal Safety");
  signal(SIGINT, signal_handler);
  for (int i = 0; i < 5; i++) {
    raise(SIGINT);
    usleep(100000);
  }
  sleep(1);
  TEST_ASSERT(signal_log_count >= 5, "Signal handler called 5 times");
  TEST_ASSERT(signal_test_complete == 1, "Signal test complete");
  signal(SIGINT, SIG_DFL);
  TEST_END("Signal Safety");
}
