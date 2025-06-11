#include "../clog.h"
#include <stdio.h>
#include <string.h>

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

extern void test_log_levels(void) {
  TEST_START("Log Levels");
  FILE *file = fopen("test_log_levels.log", "w");
  TEST_ASSERT(file != NULL, "Open log file");
  clog_set_output(file);
  clog_set_show_timestamp(0);
  clog_set_show_location(0);
  TRACE("TRACE message");
  DEBUG("DEBUG message");
  INFO("INFO message");
  WARN("WARN message");
  ERROR("ERROR message");
  FATAL("FATAL message");
  fclose(file);
  clog_set_output(NULL);
  clog_set_show_timestamp(1);
  clog_set_show_location(1);

  file = fopen("test_log_levels.log", "r");
  TEST_ASSERT(file != NULL, "Reopen log file");
  char buf[256];
  int lines = 0;
  while (fgets(buf, sizeof(buf), file)) {
    if (strstr(buf, "[TRACE] TRACE message") ||
        strstr(buf, "[DEBUG] DEBUG message") ||
        strstr(buf, "[INFO] INFO message") ||
        strstr(buf, "[WARN] WARN message") ||
        strstr(buf, "[ERROR] ERROR message") ||
        strstr(buf, "[FATAL] FATAL message")) {
      lines++;
    }
  }
  fclose(file);
  remove("test_log_levels.log");
  TEST_ASSERT(lines == 6, "All log levels logged");
  TEST_END("Log Levels");
}
