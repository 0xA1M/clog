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

extern void test_message_formatting(void) {
  TEST_START("Message Formatting");
  FILE *file = fopen("test_message_formatting.log", "w");
  TEST_ASSERT(file != NULL, "Open log file");
  clog_set_output(file);
  clog_set_show_timestamp(0);
  clog_set_show_location(0);
  INFO("String: %s", "hello");
  INFO("Number: %d", 42);
  INFO("Float: %.2f", 3.14159);
  INFO("Multiple: %s %d %.2f", "test", 123, 2.718);
  fclose(file);
  clog_set_output(NULL);
  clog_set_show_timestamp(1);
  clog_set_show_location(1);

  file = fopen("test_message_formatting.log", "r");
  TEST_ASSERT(file != NULL, "Reopen log file");
  char buf[256];
  int lines = 0;
  while (fgets(buf, sizeof(buf), file)) {
    if (strstr(buf, "[INFO] String: hello") ||
        strstr(buf, "[INFO] Number: 42") || strstr(buf, "[INFO] Float: 3.14") ||
        strstr(buf, "[INFO] Multiple: test 123 2.72")) {
      lines++;
    }
  }
  fclose(file);
  remove("test_message_formatting.log");
  TEST_ASSERT(lines == 4, "All formats logged correctly");
  TEST_END("Message Formatting");
}
