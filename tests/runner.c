#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#define usleep(t) Sleep(t / 1000)
#else
#include <unistd.h>
#endif
#ifdef _POSIX_THREADS
#include <pthread.h>
#define HAS_THREADS 1
#else
#define HAS_THREADS 0
#endif

// Forward declarations of test functions
extern void test_log_levels(void);
extern void test_message_formatting(void);
extern void test_null_pointers(void);
extern void test_long_messages(void);
extern void test_special_characters(void);
extern void test_invalid_levels(void);
extern void test_thread_safety(void);
extern void test_signal_safety(void);
extern void test_performance(void);
extern void test_configuration(void);

int main(void) {
  printf("🚀 CLOG Library Test Suite\n");
  printf("==========================\n");

  // Comment or uncomment the following lines to enable/disable specific tests
  test_log_levels();
  test_message_formatting();
  test_null_pointers();
  test_long_messages();
  test_special_characters();
  test_invalid_levels();
#if HAS_THREADS
  test_thread_safety();
#else
  printf("⚠️  Thread safety test skipped (pthread not available)\n");
#endif
  test_signal_safety();
  test_performance();
  test_configuration();

  printf("\n🎉 All tests completed successfully!\n");
  printf("=====================================\n");
  return 0;
}
