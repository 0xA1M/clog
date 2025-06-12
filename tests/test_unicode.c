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

extern void test_unicode(void) {
  TEST_START("Extensive Unicode Support");

  // Define test cases with diverse Unicode characters
  const char *test_cases[] = {
      "Latin: café",                // Latin with diacritics
      "Cyrillic: привет",           // Cyrillic script
      "Chinese: 你好",              // Chinese characters
      "Arabic: مرحبا",              // Arabic script (RTL)
      "Emoji: Hello 😊",            // Emoji
      "Combining: e\xcc\x81",       // Combining acute accent (e + U+0301)
      "RTL: שלום",                  // Hebrew script (RTL)
      "Surrogate: 🐘",              // Elephant emoji (U+1F418, surrogate pair)
      "Mixed: café😊привет",        // Mixed scripts and emoji
      "Invalid UTF-8: \xff\xfe\xfd" // Invalid UTF-8 sequence
  };
  const char *expected_lines[] = {
      "[INFO] Latin: café\n",         "[INFO] Cyrillic: привет\n",
      "[INFO] Chinese: 你好\n",       "[INFO] Arabic: مرحبا\n",
      "[INFO] Emoji: Hello 😊\n",     "[INFO] Combining: e\xcc\x81\n",
      "[INFO] RTL: שלום\n",           "[INFO] Surrogate: 🐘\n",
      "[INFO] Mixed: café😊привет\n", "[INFO] Invalid UTF-8: \xff\xfe\xfd\n"};
  const int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

  // Write to log file
  FILE *file = fopen("test_unicode.log", "w");
  TEST_ASSERT(file != NULL, "Open log file");
  clog_set_output(file);
  clog_set_show_timestamp(0);
  clog_set_show_location(0);

  for (int i = 0; i < num_cases; i++) {
    INFO("%s", test_cases[i]);
  }

  fclose(file);
  clog_set_output(NULL);
  clog_set_show_timestamp(1);
  clog_set_show_location(1);

  // Read back from log file and verify
  file = fopen("test_unicode.log", "r");
  TEST_ASSERT(file != NULL, "Reopen log file");

  char buf[256];
  int line = 0;
  while (fgets(buf, sizeof(buf), file)) {
    TEST_ASSERT(line < num_cases, "No more lines expected");
    TEST_ASSERT(strcmp(buf, expected_lines[line]) == 0, expected_lines[line]);
    line++;
  }

  fclose(file);
  remove("test_unicode.log");
  TEST_ASSERT(line == num_cases, "All test cases logged correctly");

  printf("Note: For console output, manually verify that Unicode characters "
         "(e.g., 'café', '😊') display correctly in your terminal.\n");

  TEST_END("Extensive Unicode Support");
}
