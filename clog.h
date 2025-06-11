#ifndef CLOG_H
#define CLOG_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Platform detection */
#ifdef _WIN32
#define CLOG_WINDOWS 1
#define CLOG_POSIX 0
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <fcntl.h>
#include <io.h>
#include <process.h>
#include <windows.h>
#define getpid() _getpid()
#else
#define CLOG_WINDOWS 0
#define CLOG_POSIX 1
#include <sys/types.h>
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Configuration */
#ifndef CLOG_MAX_MESSAGE_SIZE
#define CLOG_MAX_MESSAGE_SIZE 1024
#endif

#ifndef CLOG_MAX_TIME_SIZE
#define CLOG_MAX_TIME_SIZE 32
#endif

#ifndef CLOG_MAX_LOCATION_SIZE
#define CLOG_MAX_LOCATION_SIZE 256
#endif

/* Thread safety - platform-specific implementations */
#if CLOG_WINDOWS
typedef CRITICAL_SECTION clog_mutex_t;
#define CLOG_MUTEX_INIT(mutex) InitializeCriticalSection(mutex)
#define CLOG_MUTEX_LOCK(mutex) EnterCriticalSection(mutex)
#define CLOG_MUTEX_UNLOCK(mutex) LeaveCriticalSection(mutex)
#define CLOG_MUTEX_DESTROY(mutex) DeleteCriticalSection(mutex)
#define CLOG_HAS_THREADS 1
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L &&              \
    !defined(__STDC_NO_ATOMICS__)
#include <stdatomic.h>
typedef atomic_flag clog_mutex_t;
#define CLOG_MUTEX_INIT(mutex) atomic_flag_clear(mutex)
#define CLOG_MUTEX_LOCK(mutex)                                                 \
  while (atomic_flag_test_and_set(mutex)) {                                    \
  }
#define CLOG_MUTEX_UNLOCK(mutex) atomic_flag_clear(mutex)
#define CLOG_MUTEX_DESTROY(mutex) ((void)0)
#define CLOG_HAS_THREADS 1
#elif defined(__GNUC__) || defined(__clang__)
typedef volatile int clog_mutex_t;
#define CLOG_MUTEX_INIT(mutex) (*(mutex) = 0)
#define CLOG_MUTEX_LOCK(mutex)                                                 \
  while (__sync_lock_test_and_set(mutex, 1)) {                                 \
  }
#define CLOG_MUTEX_UNLOCK(mutex) __sync_lock_release(mutex)
#define CLOG_MUTEX_DESTROY(mutex) ((void)0)
#define CLOG_HAS_THREADS 1
#else
typedef int clog_mutex_t;
#define CLOG_MUTEX_INIT(mutex) (*(mutex) = 0)
#define CLOG_MUTEX_LOCK(mutex) ((void)0)
#define CLOG_MUTEX_UNLOCK(mutex) ((void)0)
#define CLOG_MUTEX_DESTROY(mutex) ((void)0)
#define CLOG_HAS_THREADS 0
#endif

/* ANSI Color Codes */
#define CLOG_RESET "\x1B[0m"
#define CLOG_BOLD "\x1B[1m"
#define CLOG_DIM "\x1B[2m"
#define CLOG_NO_BOLD "\x1B[22m"
#define CLOG_RED "\x1B[91m"
#define CLOG_YELLOW "\x1B[93m"
#define CLOG_GREEN "\x1B[92m"
#define CLOG_BLUE "\x1B[94m"
#define CLOG_WHITE "\x1B[97m"

/* Windows Console API colors (fallback for older Windows) */
#if CLOG_WINDOWS
#define CLOG_WIN_RED (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define CLOG_WIN_YELLOW                                                        \
  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define CLOG_WIN_GREEN (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define CLOG_WIN_BLUE (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define CLOG_WIN_WHITE                                                         \
  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define CLOG_WIN_DEFAULT (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#endif

/* Log Levels */
typedef enum {
  CLOG_TRACE = 0,
  CLOG_DEBUG = 1,
  CLOG_INFO = 2,
  CLOG_WARN = 3,
  CLOG_ERROR = 4,
  CLOG_FATAL = 5
} clog_level_t;

/* Color mode enumeration */
typedef enum {
  CLOG_COLOR_AUTO = 0,   /* Auto-detect color support */
  CLOG_COLOR_NEVER = 1,  /* Never use colors */
  CLOG_COLOR_ALWAYS = 2, /* Always use colors */
  CLOG_COLOR_ANSI = 3,   /* Force ANSI escape sequences */
  CLOG_COLOR_WIN32 = 4   /* Force Windows Console API */
} clog_color_mode_t;

/* Global state */
static clog_mutex_t clog_mutex;
static bool clog_mutex_initialized = false;
static clog_level_t clog_min_level = CLOG_TRACE;
static clog_color_mode_t clog_color_mode = CLOG_COLOR_AUTO;
static FILE *clog_output = NULL; /* NULL means stdout */
static bool clog_show_timestamp = true;
static bool clog_show_location = true;

#if CLOG_WINDOWS
static HANDLE clog_console_handle = INVALID_HANDLE_VALUE;
static WORD clog_original_console_attrs = 0;
static bool clog_console_initialized = false;
#endif

/* Function declarations */
static void clog_init(void);
static void clog_cleanup(void);
static const char *clog_level_string(clog_level_t level);
static const char *clog_level_color_ansi(clog_level_t level);
static void clog_format_time(char *buffer, size_t size);
static void clog_safe_write(const char *str, size_t len);

/*  __attribute__((unused)) To suppress compiler unused function warning */
static void clog_set_output(FILE *fp) __attribute__((unused));
static void clog_set_level(clog_level_t level) __attribute__((unused));
static void clog_set_color_mode(clog_color_mode_t mode) __attribute__((unused));
static void clog_set_show_timestamp(bool show) __attribute__((unused));
static void clog_set_show_location(bool show) __attribute__((unused));
static void clog_safe_strcat(char *dest, const char *src, size_t dest_size)
    __attribute__((unused));
static void clog_safe_strcpy(char *dest, const char *src, size_t dest_size)
    __attribute__((unused));

static void clog_log_impl(clog_level_t level, const char *file, int line,
                          const char *func, const char *format, va_list args);

#if CLOG_WINDOWS
static void clog_init_console(void);
static void clog_set_console_color(clog_level_t level);
static void clog_reset_console_color(void);
static int clog_is_console_output(FILE *fp);
#endif

/* Signal-safe logging function for use in signal handlers */
static inline void clog_signal_log(const char *message);

/* Main logging function */
static inline void clog_log(clog_level_t level, const char *file, int line,
                            const char *func, const char *format, ...) {
  if (level < clog_min_level)
    return;

  if (!clog_mutex_initialized) {
    clog_init();
  }

  va_list args;
  va_start(args, format);
  clog_log_impl(level, file, line, func, format, args);
  va_end(args);
}

/* Convenience macros */
#define TRACE(...)                                                             \
  clog_log(CLOG_TRACE, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define DEBUG(...)                                                             \
  clog_log(CLOG_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define INFO(...) clog_log(CLOG_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define WARN(...) clog_log(CLOG_WARN, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define ERROR(...)                                                             \
  clog_log(CLOG_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define FATAL(...)                                                             \
  clog_log(CLOG_FATAL, __FILE__, __LINE__, __func__, __VA_ARGS__)

/* Backward compatibility */
#define LOG(level, custom_error, format, ...)                                  \
  clog_log(level, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

/* Implementation */
static void clog_init(void) {
  if (clog_mutex_initialized)
    return;

  CLOG_MUTEX_INIT(&clog_mutex);
  clog_mutex_initialized = true;

#if CLOG_WINDOWS
  clog_init_console();
#endif

  /* Register cleanup function */
  atexit(clog_cleanup);
}

static void clog_cleanup(void) {
  if (!clog_mutex_initialized)
    return;

#if CLOG_WINDOWS
  if (clog_console_initialized && clog_console_handle != INVALID_HANDLE_VALUE) {
    SetConsoleTextAttribute(clog_console_handle, clog_original_console_attrs);
    clog_console_initialized = false;
  }
#endif

  CLOG_MUTEX_DESTROY(&clog_mutex);
  clog_mutex_initialized = false;
}

static void clog_set_level(clog_level_t level) { clog_min_level = level; }

static void clog_set_color_mode(clog_color_mode_t mode) {
  clog_color_mode = mode;
}

static void clog_set_output(FILE *fp) { clog_output = fp; }

static void clog_set_show_timestamp(bool show) { clog_show_timestamp = show; }

static void clog_set_show_location(bool show) { clog_show_location = show; }

static const char *clog_level_string(clog_level_t level) {
  switch (level) {
  case CLOG_TRACE:
    return "TRACE";
  case CLOG_DEBUG:
    return "DEBUG";
  case CLOG_INFO:
    return "INFO";
  case CLOG_WARN:
    return "WARN";
  case CLOG_ERROR:
    return "ERROR";
  case CLOG_FATAL:
    return "FATAL";
  default:
    return "UNKN";
  }
}

static const char *clog_level_color_ansi(clog_level_t level) {
  switch (level) {
  case CLOG_TRACE:
    return CLOG_DIM CLOG_WHITE;
  case CLOG_DEBUG:
    return CLOG_BLUE;
  case CLOG_INFO:
    return CLOG_GREEN;
  case CLOG_WARN:
    return CLOG_YELLOW;
  case CLOG_ERROR:
    return CLOG_RED;
  case CLOG_FATAL:
    return CLOG_BOLD CLOG_RED;
  default:
    return CLOG_WHITE;
  }
}

#if CLOG_WINDOWS
static void clog_init_console(void) {
  if (clog_console_initialized)
    return;

  clog_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  if (clog_console_handle != INVALID_HANDLE_VALUE) {
    CONSOLE_SCREEN_BUFFER_INFO console_info;
    if (GetConsoleScreenBufferInfo(clog_console_handle, &console_info)) {
      clog_original_console_attrs = console_info.wAttributes;
      clog_console_initialized = true;

      /* Try to enable ANSI escape sequences on Windows 10+ */
      DWORD console_mode;
      if (GetConsoleMode(clog_console_handle, &console_mode)) {
        console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(clog_console_handle, console_mode);
      }
    }
  }
}

static void clog_set_console_color(clog_level_t level) {
  if (!clog_console_initialized ||
      clog_console_handle == INVALID_HANDLE_VALUE) {
    return;
  }

  WORD color;
  switch (level) {
  case CLOG_TRACE:
    color = CLOG_WIN_DEFAULT;
    break;
  case CLOG_DEBUG:
    color = CLOG_WIN_BLUE;
    break;
  case CLOG_INFO:
    color = CLOG_WIN_GREEN;
    break;
  case CLOG_WARN:
    color = CLOG_WIN_YELLOW;
    break;
  case CLOG_ERROR:
    color = CLOG_WIN_RED;
    break;
  case CLOG_FATAL:
    color = CLOG_WIN_RED | BACKGROUND_RED;
    break;
  default:
    color = CLOG_WIN_WHITE;
    break;
  }

  SetConsoleTextAttribute(clog_console_handle, color);
}

static void clog_reset_console_color(void) {
  if (clog_console_initialized && clog_console_handle != INVALID_HANDLE_VALUE) {
    SetConsoleTextAttribute(clog_console_handle, clog_original_console_attrs);
  }
}

static int clog_is_console_output(FILE *fp) {
  if (!fp)
    fp = stdout;

  /* Check if output is going to console */
  int fd = _fileno(fp);
  return _isatty(fd);
}
#endif

/* Cross-platform time formatting */
static void clog_format_time(char *buffer, size_t size) {
  time_t now;
  struct tm *tm_info;

  now = time(NULL);
  if (now == (time_t)-1) {
    strncpy(buffer, "0000-00-00 00:00:00", size - 1);
    buffer[size - 1] = '\0';
    return;
  }

#if CLOG_WINDOWS
  /* Windows thread-safe version */
  struct tm tm_storage;
  if (localtime_s(&tm_storage, &now) != 0) {
    strncpy(buffer, "0000-00-00 00:00:00", size - 1);
    buffer[size - 1] = '\0';
    return;
  }
  tm_info = &tm_storage;
#else
  /* POSIX thread-safe version */
  struct tm tm_storage;
  tm_info = localtime_r(&now, &tm_storage);
  if (!tm_info) {
    strncpy(buffer, "0000-00-00 00:00:00", size - 1);
    buffer[size - 1] = '\0';
    return;
  }
#endif

  if (strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info) == 0) {
    strncpy(buffer, "0000-00-00 00:00:00", size - 1);
    buffer[size - 1] = '\0';
  }
}

/* Safe write function */
static void clog_safe_write(const char *str, size_t len) {
  FILE *output = clog_output ? clog_output : stdout;

  if (fwrite(str, 1, len, output) != len) {
    /* Error handling is limited in signal-safe context */
    exit(EXIT_FAILURE);
  }
  fflush(output);
}

/* Signal-safe logging implementation */
static inline void clog_signal_log(const char *message) {
#if CLOG_POSIX
  size_t len = strlen(message);
  write(STDERR_FILENO, message, len);
  write(STDERR_FILENO, "\n", 1);
#else /* CLOG_WINDOWS */
  size_t len = strlen(message);
  _write(_fileno(stderr), message, len);
  _write(_fileno(stderr), "\n", 1);
#endif
}

/* Safe string length calculation with maximum limit */
static size_t clog_safe_strlen(const char *str, size_t max_len) {
  if (!str)
    return 0;
  size_t len = 0;
  while (len < max_len && str[len] != '\0') {
    len++;
  }
  return len;
}

/* Safe string copy */
static void clog_safe_strcpy(char *dest, const char *src, size_t dest_size) {
  if (!dest || dest_size == 0)
    return;
  if (!src) {
    dest[0] = '\0';
    return;
  }

  size_t i = 0;
  while (i < dest_size - 1 && src[i] != '\0') {
    dest[i] = src[i];
    i++;
  }
  dest[i] = '\0';
}

/* Safe string concatenation */
static void clog_safe_strcat(char *dest, const char *src, size_t dest_size) {
  if (!dest || !src || dest_size == 0)
    return;

  size_t dest_len = clog_safe_strlen(dest, dest_size);
  if (dest_len >= dest_size - 1)
    return;

  size_t remaining = dest_size - dest_len - 1;
  size_t i = 0;

  while (i < remaining && src[i] != '\0') {
    dest[dest_len + i] = src[i];
    i++;
  }
  dest[dest_len + i] = '\0';
}

/* Extract filename from path (handles both / and \ separators) */
static const char *clog_basename(const char *path) {
  if (!path)
    return "unknown";

  const char *filename = path;
  const char *p = path;

  while (*p) {
    if (*p == '/' || *p == '\\') {
      filename = p + 1;
    }
    p++;
  }

  return filename;
}

/* Determine if colors should be used */
static bool clog_should_use_colors(void) {
  FILE *output = clog_output ? clog_output : stdout;

  switch (clog_color_mode) {
  case CLOG_COLOR_NEVER:
    return false;
  case CLOG_COLOR_ALWAYS:
  case CLOG_COLOR_ANSI:
  case CLOG_COLOR_WIN32:
    return true;
  case CLOG_COLOR_AUTO:
  default:
#if CLOG_WINDOWS
    return clog_is_console_output(output);
#else
    return isatty(fileno(output));
#endif
  }
}

/* Determine color implementation to use */
static bool clog_use_ansi_colors(void) {
  if (!clog_should_use_colors())
    return false;

  switch (clog_color_mode) {
  case CLOG_COLOR_ANSI:
    return true;
  case CLOG_COLOR_WIN32:
    return false;
  case CLOG_COLOR_AUTO:
  case CLOG_COLOR_ALWAYS:
  default:
#if CLOG_WINDOWS
    /* On Windows 10+, prefer ANSI if available, otherwise use Console API */
    return true; /* Modern Windows supports ANSI */
#else
    return true; /* POSIX systems use ANSI */
#endif
  }
}

/* Main logging implementation */
static void clog_log_impl(clog_level_t level, const char *file, int line,
                          const char *func, const char *format, va_list args) {
  /* Pre-allocated buffers to avoid dynamic allocation */
  static char time_buf[CLOG_MAX_TIME_SIZE];
  static char message_buf[CLOG_MAX_MESSAGE_SIZE];
  static char location_buf[CLOG_MAX_LOCATION_SIZE];
  static char final_buf[CLOG_MAX_MESSAGE_SIZE + CLOG_MAX_TIME_SIZE +
                        CLOG_MAX_LOCATION_SIZE + 256];

  if (!clog_mutex_initialized)
    clog_init();

  /* Acquire lock for thread safety */
  CLOG_MUTEX_LOCK(&clog_mutex);

  /* Format timestamp */
  clog_format_time(time_buf, sizeof(time_buf));

  /* Format the user message */
  vsnprintf(message_buf, sizeof(message_buf), format, args);

  /* Format location info if provided */
  location_buf[0] = '\0';
  if (file && line > 0 && func) {
    snprintf(location_buf, sizeof(location_buf), " (%s:%d in %s)",
             clog_basename(file), line, func);
  }

  /* Determine color settings */
  bool use_colors = clog_should_use_colors();
  (void)use_colors; // Suppress compiler unused variable warning
  bool use_ansi = clog_use_ansi_colors();

  /* Build the final log line */
  size_t len = 0;

  /* Step 1: Add timestamp if enabled */
  if (clog_show_timestamp) {
    if (use_ansi) {
      len += snprintf(final_buf + len, sizeof(final_buf) - len, "%s%s%s ",
                      CLOG_BOLD, time_buf, CLOG_NO_BOLD);
    } else {
      len +=
          snprintf(final_buf + len, sizeof(final_buf) - len, "%s ", time_buf);
    }
  }

  /* Step 2: Add colored log level */
  if (use_ansi) {
    len += snprintf(final_buf + len, sizeof(final_buf) - len, "%s[%s]%s ",
                    clog_level_color_ansi(level), clog_level_string(level),
                    CLOG_RESET);
  } else {
    len += snprintf(final_buf + len, sizeof(final_buf) - len, "[%s] ",
                    clog_level_string(level));
  }

  /* Step 3: Add log message */
  len += snprintf(final_buf + len, sizeof(final_buf) - len, "%s", message_buf);

  /* Step 4: Add location if enabled */
  if (clog_show_location && location_buf[0] != '\0') {
    if (use_ansi) {
      len += snprintf(final_buf + len, sizeof(final_buf) - len, " %s%s%s",
                      CLOG_DIM, location_buf, CLOG_RESET);
    } else {
      len += snprintf(final_buf + len, sizeof(final_buf) - len, " %s",
                      location_buf);
    }
  }

  /* Step 5: Add newline */
  len += snprintf(final_buf + len, sizeof(final_buf) - len, "\n");

  /* For Windows Console API */
#if CLOG_WINDOWS
  if (use_colors && !use_ansi) {
    clog_set_console_color(level);
  }
#endif

  /* Write the log line */
  clog_safe_write(final_buf, len);

  /* Reset console color if using Windows Console API */
#if CLOG_WINDOWS
  if (use_colors && !use_ansi) {
    clog_reset_console_color();
  }
#endif

  /* Release lock */
  CLOG_MUTEX_UNLOCK(&clog_mutex);
}

#ifdef __cplusplus
}
#endif

#endif /* CLOG_H */
