#ifndef LOGGER_H
#define LOGGER_H

/* Reset Escape Sequence */
#define RESET "\x1B[0m" // Resets all previous ANSI escape codes

/* Font Style Escape Sequences */
#define STYLE_DIM "\x1B[2m"      // Sets text to dim (faded) style
#define STYLE_NO_DIM "\x1B[22m"  // Resets dim style
#define STYLE_BOLD "\x1B[1m"     // Sets text to bold style
#define STYLE_NO_BOLD "\x1B[22m" // Resets bold style (also resets dim)

/* Font Color Escape Sequences */
#define ERR_COLOR "\x1B[91m"   // Bright Red color (used for errors)
#define WARN_COLOR "\x1B[93m"  // Yellow color (used for warnings)
#define INFO_COLOR "\x1B[92m"  // Light Green color (used for info messages)
#define DEBUG_COLOR "\x1B[94m" // Light Blue color (used for debug messages)
#define MSG_COLOR "\x1B[97m"   // Bright White color (used for log messages)

/* Enum to Define Log Levels */
typedef enum {
  INFO, // Informational messages
  WARN, // Warnings that might indicate a potential problem
  ERR,  // Errors that indicate something went wrong
  DBG   // Debugging information, typically for developers
} log_level_t;

/**
 * @brief Logs a message with the specified log level, optional error message,
 *        and contextual information (file, line, function).
 *
 * @param level The log level (INFO, WARNING, ERROR, DEBUG).
 * @param custom_error An optional custom error message (can be NULL).
 * @param file The source file where the log was generated.
 * @param line The line number in the source file.
 * @param func The function name where the log was generated.
 * @param format The format string for the log message.
 * @param ... Additional arguments for the format string.
 */
void log_message(log_level_t level, const char *custom_error, const char *file,
                 int line, const char *func, const char *format, ...);

/**
 * @brief Cleans up the logger resources.
 * This function should be called when the logger is no longer needed,
 * typically at program exit.
 */
void logger_cleanup(void);

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

/**
 * @brief Macro for Easier Logging with File, Line, and Function Info
 *
 * The LOG macro simplifies the logging process by automatically including
 * the file name, line number, and function name where the logging call is made.
 * It also allows for custom error messages and supports variadic arguments
 * for formatting log messages.
 *
 * @params:
 * - level: The logging level (e.g., INFO, WARN, ERR).
 * - custom_error: A custom error message or context for the log entry.
 * - format: A format string similar to printf, specifying how the rest of the
 *   arguments should be formatted in the log message.
 * - ...: Additional arguments used to fill in the format string.
 *
 */
#define LOG(level, custom_error, format, ...)                                  \
  log_message(level, custom_error, __FILE__, __LINE__, __func__, format,       \
              ##__VA_ARGS__)

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

/**
 * @examples: Examples of using the `LOG` macro for various log levels.
 *
 * 1. **Logging an Informational Message**
 *
 *    ```c
 *    #include "logger.h"
 *
 *    int main() {
 *        // Log an informational message
 *        LOG(LOG_INFO, NULL, "This is an info message with value: %d\n", 42);
 *        return 0;
 *    }
 *    ```
 *
 * 2. **Logging a Warning Message**
 *
 *    ```c
 *    #include "logger.h"
 *
 *    int main() {
 *        // Simulate a condition where a warning should be logged
 *        int warning_code = 1; // Example warning code
 *
 *        // Log a warning message with code
 *        LOG(LOG_WARNING, NULL, "This is a warning with code: %d\n",
 * warning_code); return 0;
 *    }
 *    ```
 *
 * 3. **Logging an Error Message with `errno`**
 *
 *    ```c
 *    #include "logger.h"
 *    #include <errno.h>
 *
 *    int main() {
 *        // Simulate an error condition
 *        FILE *file = fopen("nonexistent_file.txt", "r");
 *        if (!file) {
 *            // Log an error message including `errno`
 *            LOG(LOG_ERROR, NULL, "Failed to open file\n");
 *        }
 *        return 0;
 *    }
 *    ```
 *
 * 4. **Logging an Error Message with a Custom Error**
 *
 *    ```c
 *    #include "logger.h"
 *    #include <netdb.h> // For gai_strerror
 *
 *    int main() {
 *        // Simulate a getaddrinfo failure
 *        int errcode = EAI_AGAIN; // Example error code
 *        const char *custom_err = gai_strerror(errcode);
 *
 *        // Log an error message with a custom error message
 *        LOG(LOG_ERROR, custom_err, "Failed to resolve hostname\n");
 *        return 0;
 *    }
 *    ```
 *
 * 5. **Logging a Debug Message**
 *
 *    ```c
 *    #include "logger.h"
 *
 *    int main() {
 *        // Log a debug message with additional information
 *        int debug_value = 123;
 *        LOG(LOG_DEBUG, NULL, "Debugging value: %d\n", debug_value);
 *        return 0;
 *    }
 *    ```
 */

#endif /* LOGGER_H */
