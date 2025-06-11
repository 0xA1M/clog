#include "../clog.h"
#include <stdio.h>

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

extern void test_invalid_levels(void) {
  TEST_START("Invalid Levels");
  FILE *file = fopen("test_invalid_levels.log", "w");
  TEST_ASSERT(file != NULL, "Open log file");
  clog_set_output(file);
  clog_set_show_timestamp(0);
  clog_set_show_location(0);
  clog_log(-1, __FILE__, __LINE__, __func__, "Invalid level %d", -1);
  clog_log(6, __FILE__, __LINE__, __func__, "Invalid level %d", 6);
  fclose(file);
  clog_set_output(NULL);
  clog_set_show_timestamp(1);
  clog_set_show_location(1);

  file = fopen("test_invalid_levels.log", "r");
  TEST_ASSERT(file != NULL, "Reopen log file");
  char buf[256];
  int lines = 0;
  while (fgets(buf, sizeof(buf), file)) {
    if (strstr(buf, "[UNKN] Invalid level")) {
      lines++;
    }
  }
  fclose(file);
  remove("test_invalid_levels.log");
  TEST_ASSERT(lines == 2, "Invalid levels logged as UNKN");
  TEST_END("Invalid Levels");
}
