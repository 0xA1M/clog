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

extern void test_null_pointers(void) {
  TEST_START("Null Pointers");
  FILE *file = fopen("test_null_pointers.log", "w");
  TEST_ASSERT(file != NULL, "Open log file");
  clog_set_output(file);
  clog_set_show_timestamp(0);
  clog_set_show_location(0);
  INFO("NULL string: %s", (char *)NULL);
  fclose(file);
  clog_set_output(NULL);
  clog_set_show_timestamp(1);
  clog_set_show_location(1);

  file = fopen("test_null_pointers.log", "r");
  TEST_ASSERT(file != NULL, "Reopen log file");
  char buf[256];
  int lines = 0;
  while (fgets(buf, sizeof(buf), file)) {
    if (strstr(buf, "[INFO] NULL string: ")) {
      lines++;
    }
  }
  fclose(file);
  remove("test_null_pointers.log");
  TEST_ASSERT(lines == 1, "NULL pointer handled safely");
  TEST_END("Null Pointers");
}
