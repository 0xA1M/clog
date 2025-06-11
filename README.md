# clog: A Lightweight, Thread‑Safe, Colorful C Logging Library

`clog` is a single‑header, thread‑safe logging library for C, offering timestamped, context‑rich output with configurable levels and color modes. It works on POSIX and Windows, and requires no dynamic allocations in the hot path.

## Features

- **Log Levels**: TRACE, DEBUG, INFO, WARN, ERROR, FATAL
- **Thread Safety**:
  Uses Windows Critical Sections, C11 atomics, or GCC/Clang spin-locks; fallback to no-op in single-threaded builds
- **Signal Safety**:
  Provides a minimal, async-signal-safe logging function `clog_signal_log` for use in signal handlers
- **Color Modes**:
  - Auto-detect (ANSI on POSIX TTYs or modern Windows consoles)
  - Force ANSI escapes
  - Force Windows Console API
  - Always/never color
- **Contextual Output**:
  Outputs timestamp, level tag, message, and optional `(file:line in function)` location info
- **Customizable**:
  - `clog_set_level(...)` to filter by minimum level
  - `clog_set_color_mode(...)` to override color behavior
  - `clog_set_output(...)` to redirect logs to any `FILE*`
- **Performance-oriented**:
  Pre-allocated buffers, no `malloc` in log path
- **Portable**:
  Works on Linux, macOS, Windows (compatible with MSVC, GCC, Clang)

## Getting Started

### Requirements

- C compiler supporting C11 (optional)
- POSIX or Win32 API for threading and console handling

### Installation

```sh
git clone https://github.com/0xA1M/clog.git
cd clog
````

Then copy `clog.h` into your project:

```c
#include "clog.h"
```

## Usage

### Basic Logging

```c
INFO("Server started on port %d", port);
WARN("Low disk space: %d%% remaining", percent);
ERROR("Failed to open file: %s", strerror(errno));
DEBUG("Cache hit for key '%s'", key);
TRACE("Entering function %s", __func__);
FATAL("Unexpected null pointer");
```

### Configuration

Set minimum log level (default is INFO):

```c
clog_set_level(CLOG_DEBUG);
```

Control color output (default is `CLOG_COLOR_AUTO`):

```c
clog_set_color_mode(CLOG_COLOR_NEVER);
clog_set_color_mode(CLOG_COLOR_ALWAYS);
clog_set_color_mode(CLOG_COLOR_ANSI);
clog_set_color_mode(CLOG_COLOR_WIN32);
```

Redirect log output (default is `stdout`):

```c
FILE *fp = fopen("app.log", "w");
clog_set_output(fp);
// ... logging calls ...
fclose(fp);
```

> ⚠️ **Note**: If you redirect output to a file, you should disable colors using `clog_set_color_mode(CLOG_COLOR_NEVER)` as ANSI escape codes may not render properly in files.
> ✅ **TODO**: Improve color handling for file outputs.

Cleanup (optional, automatically called via `atexit`):

```c
clog_cleanup();
```

## API Reference

```c
void clog_set_level(clog_level_t level);
void clog_set_color_mode(clog_color_mode_t mode);
void clog_set_output(FILE *fp);
void clog_cleanup(void);
void clog_signal_log(const char *message); // Async-signal-safe, minimal logging to stderr

enum clog_level_t {
    CLOG_TRACE, CLOG_DEBUG, CLOG_INFO,
    CLOG_WARN,  CLOG_ERROR, CLOG_FATAL
};

enum clog_color_mode_t {
    CLOG_COLOR_AUTO, CLOG_COLOR_NEVER,
    CLOG_COLOR_ALWAYS, CLOG_COLOR_ANSI,
    CLOG_COLOR_WIN32
};
```

All log macros expand to `clog_log(...)` and automatically capture file, line, and function.

## Building & Testing

The test suite lives under `tests/` and covers:

* Basic functionality and level filtering
* Memory-safety and buffer limits
* Signal-handler safety
* Thread-safety (requires pthreads)
* Performance benchmarks
* Configuration and output redirection

### Build & Run

```sh
make test
```

## Compatibility

✅ `clog` is **tested on Linux** (x86\_64) with:

* **GCC**, **Clang** compilers
* **glibc** and **musl** C libraries
* Both **multi-threaded** and **single-threaded** builds
* Modern **terminal emulators** with ANSI support (GNOME Terminal, Alacritty, etc.)

⚠️ Support for **Windows** is implemented but not yet fully tested in production.
If you use it on Windows, feedback and patches are welcome!

## Examples

See `tests/runner.c` for full test coverage and usage examples.

## Contributing

1. Fork the repo
2. Create your branch: `git checkout -b feature/foo`
3. Commit your changes
4. Push: `git push origin feature/foo`
5. Open a Pull Request

✅ Please include tests and make sure all tests pass before submitting.

## License

MIT License. See [LICENSE](LICENSE) for details.
