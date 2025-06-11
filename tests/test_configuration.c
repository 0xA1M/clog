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

extern void test_configuration(void) {
  TEST_START("Configuration");
  FILE *file = fopen("test_no_timestamp.log", "w");
  TEST_ASSERT(file != NULL, "Open log file for no timestamp");
  clog_set_output(file);
  clog_set_show_timestamp(0);
  INFO("Message without timestamp");
  fclose(file);
  clog_set_output(NULL);
  clog_set_show_timestamp(1);

  file = fopen("test_no_timestamp.log", "r");
  TEST_ASSERT(file != NULL, "Reopen log file for no timestamp");
  char buf[256];
  fgets(buf, sizeof(buf), file);
  fclose(file);
  remove("test_no_timestamp.log");
  TEST_ASSERT(strstr(buf, "[INFO] Message without timestamp") != NULL,
              "No timestamp present");

  file = fopen("test_no_location.log", "w");
  TEST_ASSERT(file != NULL, "Open log file for no location");
  clog_set_output(file);
  clog_set_show_location(0);
  INFO("Message without location");
  fclose(file);
  clog_set_output(NULL);
  clog_set_show_location(1);

  file = fopen("test_no_location.log", "r");
  TEST_ASSERT(file != NULL, "Reopen log file for no location");
  fgets(buf, sizeof(buf), file);
  fclose(file);
  remove("test_no_location.log");
  TEST_ASSERT(strchr(buf, '(') == NULL, "No location present");

  file = fopen("cfg_notime_noloc.log", "w");
  TEST_ASSERT(file != NULL, "Open file for no time and no location");
  clog_set_output(file);
  clog_set_show_timestamp(0);
  clog_set_show_location(0);
  INFO("No time and loc");
  fclose(file);
  clog_set_output(NULL);
  // restore defaults
  clog_set_show_timestamp(1);
  clog_set_show_location(1);
  file = fopen("cfg_notime_noloc.log", "r");
  fgets(buf, sizeof(buf), file);
  fclose(file);
  TEST_ASSERT(strstr(buf, "[INFO] No time and loc") != NULL,
              "Message with only level and text");
  TEST_ASSERT(strcmp(strchr(buf, ' '), buf),
              "No timestamp or location formatting");
  remove("cfg_notime_noloc.log");

  file = fopen("test_color_modes.log", "w");
  TEST_ASSERT(file != NULL, "Open log file for color modes");
  clog_set_output(file);
  clog_set_color_mode(CLOG_COLOR_NEVER);
  INFO("No color message");
  clog_set_color_mode(CLOG_COLOR_ALWAYS);
  INFO("Colored message");
  remove("test_color_modes.log");
  fclose(file);
}
