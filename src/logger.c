#include <errno.h>   // For handling system errors and using errno
#include <netdb.h>   // For handling network errors and using gai_strerror()
#include <pthread.h> // For thread safety
#include <stdarg.h> // For handling variable arguments (va_list, va_start, etc.)
#include <stdio.h>  // For standard I/O functions (printf, vprintf, etc.)
#include <stdlib.h> // For free
#include <string.h> // For strerror (converts errno to a human-readable string)
#include <time.h>   // For handling and formatting time

#include "logger.h"

static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Retrieves the current system time as a string in the format
 *        "YYYY/MM/DD HH:MM:SS".
 *
 * @param buffer The buffer to store the formatted time string.
 * @param buffer_size The size of the buffer.
 */
static void get_current_time(char *buffer, size_t buffer_size) {
  time_t raw_time;
  struct tm time_info;

  // Get the current system time
  time(&raw_time);

  // Convert it to local time (based on system's timezone)
  // Thread-safe version of localtime
  localtime_r(&raw_time, &time_info);

  // Format the time as "YYYY/MM/DD HH:MM:SS"
  strftime(buffer, buffer_size, "%Y/%m/%d %H:%M:%S", &time_info);
}

void log_message(log_level_t level, const char *custom_err, const char *file,
                 int line, const char *func, const char *format, ...) {
  va_list args;
  va_start(args, format); // Initialize the argument list

  int err_code = errno;

  char time_buffer[20];
  get_current_time(time_buffer, sizeof(time_buffer));

  char *message = NULL;
  int message_len = 0;

  // First, try to format the message to determine its length
  va_list args_copy;
  va_copy(args_copy, args);
  message_len = vsnprintf(NULL, 0, format, args_copy) + 1;
  va_end(args_copy);

  // Allocate memory for the message
  message = (char *)malloc(message_len);
  if (!message) {
    perror("Failed to allocate memory for log message\n");
    va_end(args);
    exit(EXIT_FAILURE);
  }

  // Actually format the message
  vsnprintf(message, message_len, format, args);
  va_end(args);

  // Lock the mutex before writing to stdout
  if (pthread_mutex_lock(&log_mutex) != 0) {
    fprintf(stderr, "Failed to lock mutex for logging\n");
    free(message);
    exit(EXIT_FAILURE);
  }

  printf(STYLE_BOLD "%s" STYLE_NO_BOLD " ", time_buffer);
  // Print the log level-specific annotation and color
  switch (level) {
  case INFO:
    printf(STYLE_BOLD INFO_COLOR "[INFO] " STYLE_NO_BOLD MSG_COLOR);
    break;
  case WARN:
    printf(STYLE_BOLD WARN_COLOR "[WARNING] " STYLE_NO_BOLD MSG_COLOR);
    printf(STYLE_DIM "(%s:%d in %s) " STYLE_NO_DIM, file, line, func);
    break;
  case ERR:
    printf(STYLE_BOLD ERR_COLOR "[ERROR] " STYLE_NO_BOLD MSG_COLOR);
    printf(STYLE_DIM "(%s:%d in %s) " STYLE_NO_DIM, file, line, func);
    break;
  case DBG:
    printf(STYLE_BOLD DEBUG_COLOR "[DEBUG] " STYLE_NO_BOLD MSG_COLOR);
    printf(STYLE_DIM "(%s:%d in %s) " STYLE_NO_DIM, file, line, func);
    break;
  }

  // Print the user-provided log message (format string and arguments)
  printf("%s", message);

  // If the log level is ERROR or WARN, print a custom error message
  // or strerror(errno)
  if (level == ERR) {
    if (custom_err)
      printf(": %s", custom_err);
    else
      printf(": %s", strerror(err_code));
  } else if (level == WARN) {
    printf(": %s", strerror(err_code));
  }

  printf(RESET "\n");
  fflush(stdout);

  if (pthread_mutex_unlock(&log_mutex) != 0) {
    fprintf(stderr, "Failed to unlock mutex after logging\n");
  }

  free(message);
}

void logger_cleanup(void) { pthread_mutex_destroy(&log_mutex); }
