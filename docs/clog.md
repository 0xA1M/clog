# clog Documentation

## Table of Contents
- [Overview](#overview)
- [Installation](#installation)
- [Usage](#usage)
  - [Log Levels](#log-levels)
  - [Macros](#macros)
  - [Logging Functions](#logging-functions)
  - [Helper Functions](#helper-functions)
  - [Cleaning Up](#cleaning-up)
- [Thread Safety](#thread-safety)
- [Customization](#customization)
- [Examples](#examples)
- [Error Handling](#error-handling)

---

## Overview

**clog** is a simple yet flexible C logging library designed to assist with logging messages in multithreaded applications. It supports colored and timestamped logs, multiple log levels (`INFO`, `WARN`, `ERR`, `DBG`), and offers user-friendly macros to simplify adding contextual information (file, line, and function name) to logs.

---

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/0xA1M/clog
    ```
2. Add `clog.h` and `clog.c` to your project.
3. Include `clog.h` in your source code:
    ```c
    #include "clog.h"
    ```

---

## Usage

### Log Levels

`clog` supports the following log levels:

| Level | Description                              |
|-------|------------------------------------------|
| INFO  | General informational messages.          |
| WARN  | Warnings that indicate potential issues. |
| ERR   | Critical errors with optional `errno`.   |
| DBG   | Debugging information for developers.    |

### Macros

`clog` provides an easy-to-use `LOG` macro that automatically includes contextual information (file, line number, and function name) in the log messages.

#### LOG Macro

```c
#define LOG(level, custom_error, format, ...)
```

- **level**: The log level (e.g., `INFO`, `WARN`, `ERR`, `DBG`).
- **custom_error**: A custom error message or `NULL`.
- **format**: The format string (similar to `printf`).
- **...**: Additional arguments for the format string.

### Logging Functions

#### log_message

```c
void log_message(log_level_t level, const char *custom_err, const char *file, int line, const char *func, const char *format, ...);
```

- **level**: Log level (`INFO`, `WARN`, `ERR`, `DBG`).
- **custom_err**: Custom error message. If `NULL`, `errno` will be used for `ERR` and `WARN` levels.
- **file**: The source file where the log was generated.
- **line**: The line number in the source file.
- **func**: The function name where the log was generated.
- **format**: `printf`-style format string for the log message.
- **...**: Additional arguments for the format string.

This function handles the main logging logic:
1. Obtains the current timestamp.
2. Formats the user-provided message using `vsnprintf`.
3. If the log level is `ERR` or `WARN`, appends a custom error message or the string from `strerror(errno)`.
4. Outputs the message to `stdout`, ensuring thread safety with `pthread_mutex_t` locking.

##### Example

```c
LOG(INFO, NULL, "Starting application version %s", "1.0.0");
LOG(ERR, "Failed to connect to database", "Cannot connect to %s", "database");
```

#### logger_cleanup

```c
void logger_cleanup(void);
```

This function cleans up resources used by the logger. It is essential to call `logger_cleanup()` at the end of your program to release the mutex initialized for thread safety.

---

### Helper Functions

#### get_current_time

```c
static void get_current_time(char *buffer, size_t buffer_size);
```

This function formats the current system time as a string in the format `YYYY/MM/DD HH:MM:SS` and stores it in the provided `buffer`. It uses `localtime_r` to ensure thread-safe conversion from `time_t` to `tm` structure.

- **buffer**: A pointer to the buffer where the formatted time string will be stored.
- **buffer_size**: The size of the buffer.

---

## Thread Safety

The **clog** library is designed to be thread-safe. It achieves this by using a static `pthread_mutex_t log_mutex` to serialize access to the log output. This ensures that even when multiple threads attempt to log messages simultaneously, the logs will not interleave, preserving their readability.

### Mutex Locking

The logger uses `pthread_mutex_lock()` to acquire the mutex before writing to `stdout` and `pthread_mutex_unlock()` to release it afterward. If locking or unlocking fails, the logger outputs an error message and terminates the program.

---

## Customization

You can customize the following aspects of **clog**:

### Log Colors

The colors used in log messages are defined using ANSI escape codes. These can be easily modified in `logger.h`:

```c
#define ERR_COLOR "\x1B[91m"   // Bright red for errors
#define WARN_COLOR "\x1B[93m"  // Yellow for warnings
#define INFO_COLOR "\x1B[92m"  // Light green for info messages
#define DEBUG_COLOR "\x1B[94m" // Light blue for debug messages
#define MSG_COLOR "\x1B[97m"   // Bright white for log messages
```

### Font Styles

You can also modify font styles, such as bold or dim text:

```c
#define STYLE_BOLD "\x1B[1m"
#define STYLE_DIM "\x1B[2m"
#define RESET "\x1B[0m" // Reset all styles
```

---

## Examples

Here are examples of using **clog** for various purposes:

### Example 1: Logging Information

```c
#include "clog.h"

int main() {
    LOG(INFO, NULL, "Application started successfully.");
    LOG(INFO, NULL, "User logged in with ID: %d", 101);
    logger_cleanup();
    return 0;
}
```

### Example 2: Logging Warnings

```c
#include "clog.h"

int main() {
    int ret = some_function();
    if (ret != 0) {
        LOG(WARN, NULL, "Non-critical issue detected: %d", ret);
    }
    logger_cleanup();
    return 0;
}
```

### Example 3: Logging Errors

```c
#include "clog.h"
#include <errno.h>

int main() {
    FILE *file = fopen("nonexistent_file.txt", "r");
    if (!file) {
        LOG(ERR, NULL, "Failed to open file");
    }
    logger_cleanup();
    return 0;
}
```

### Example 4: Debug Logging

```c
#include "clog.h"

int main() {
    int value = 42;
    LOG(DBG, NULL, "Debugging value: %d", value);
    logger_cleanup();
    return 0;
}
```

---

## Error Handling

In **clog**, errors during logging are generally non-recoverable (e.g., failure to allocate memory or lock the mutex), and the library outputs an error message and terminates the program using `exit(EXIT_FAILURE)`.

For custom error handling, you can pass a custom error message to `LOG()` or let **clog** handle it automatically by reporting the error using `strerror(errno)`.

---

## Cleaning Up

To ensure that **clog** releases all resources (mainly the `pthread_mutex_t` used for thread safety), you should call the `logger_cleanup()` function at the end of your program:

```c
logger_cleanup();
```

Failing to call this function may result in resource leaks in long-running or multithreaded applications.
