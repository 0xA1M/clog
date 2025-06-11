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

extern void test_long_messages(void) {
  TEST_START("Long Messages");
  clog_set_show_timestamp(0);
  clog_set_show_location(0);
  char long_msg[2048];
  memset(long_msg, 'A', sizeof(long_msg) - 1);
  long_msg[sizeof(long_msg) - 1] = '\0';
  FILE *file = fopen("test_long_messages.log", "w");
  TEST_ASSERT(file != NULL, "Open log file");
  clog_set_output(file);
  INFO("Long message: %s", long_msg);
  fclose(file);
  clog_set_output(NULL);
  clog_set_show_timestamp(1);
  clog_set_show_location(1);

  file = fopen("test_long_messages.log", "r");
  TEST_ASSERT(file != NULL, "Reopen log file");
  char buf[2048];
  fgets(buf, sizeof(buf), file);
  fclose(file);
  remove("test_long_messages.log");
  const char *prefix = "[INFO] Long message: ";
  size_t prefix_len = strlen(prefix);
  TEST_ASSERT(strncmp(buf, prefix, prefix_len) == 0, "Prefix matches");
  size_t total_len = strlen(buf);
  size_t expected_len = prefix_len + 1009 + 1; // 1009 'A's + '\n'
  TEST_ASSERT(total_len == expected_len, "Message truncated correctly");
  TEST_END("Long Messages");
}
