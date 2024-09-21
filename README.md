# clog: A Lightweight, Thread-Safe Logging Library for C

`clog` is a simple, thread-safe logging library for C that supports multiple log levels (INFO, WARN, ERROR, DEBUG) and customizable output formatting. It can be used in both single-threaded and multi-threaded applications to provide detailed, context-rich logging.

## Features
- Thread-safe logging with `pthread_mutex`
- Support for multiple log levels:
  - **INFO**: Informational messages
  - **WARN**: Warnings that may indicate potential problems
  - **ERR**: Error messages that indicate failures
  - **DBG**: Debugging messages for development
- Contextual information included in logs (file, line number, function)
- ANSI color-coded log output for enhanced readability
- Customizable error messages or automatic integration with `errno` for error handling
- Extensive documentation and examples

## Getting Started

### Requirements
- GCC or Clang compiler
- POSIX-compatible system (for `pthread` and `errno`)

### Installation

Clone the repository to your local system:

```bash
git clone https://github.com/0xA1M/clog
cd clog
```

### Usage

You can include the logging library in your project by including the `clog.h` file:

```c
#include "clog.h"
```

To log messages with varying severity levels, use the `LOG` macro:

```c
LOG(INFO, NULL, "This is an informational message.");
LOG(WARN, NULL, "This is a warning.");
LOG(ERR, "Custom error message", "This is an error.");
LOG(DBG, NULL, "Debugging message: variable value is %d", some_variable);
```

### Example

```c
#include "clog.h"

int main() {
    LOG(INFO, NULL, "This is an informational message.");
    LOG(WARN, NULL, "This is a warning.");
    LOG(ERR, NULL, "This is an error with errno: %s", strerror(errno));
    LOG(DBG, NULL, "Debug message: %d", 42);

    return 0;
}
```

Compile and run your program:

```bash
gcc -pthread -o my_program my_program.c src/clog.c
./my_program
```

## Documentation

An extensive documentation of the `clog` library is available in the `docs/` folder. The documentation includes:
- Detailed descriptions of the library's features
- How the logging system works
- Function reference
- Examples and use cases

### Accessing the Documentation

You can explore the documentation locally by navigating to the `docs/` folder, or view it online if available.

```bash
cd docs/
```

## Running Tests

The `clog` library comes with a set of unit tests to ensure its core functionality and thread safety. These tests are located in the `tests/` folder and cover:
- Basic logging
- Error handling (with and without custom messages)
- Thread safety in multi-threaded environments

### Running the Test Suite

1. Ensure you have the test dependencies installed.
2. Run the tests using the `Makefile`.

```bash
make test
```

This will compile and execute all test cases, verifying that the clog functions as expected.

### Test Structure

- Tests are located in the `tests/` folder.
- The `Makefile` will compile the tests and place executables in the `build/` directory.
- Each test executable is run to validate different aspects of the library.

## Contributing

Contributions are welcome! If you'd like to contribute, please follow these steps:
1. Fork the repository.
2. Create a new branch (`git checkout -b my-feature-branch`).
3. Make your changes.
4. Commit your changes (`git commit -m "Add some feature"`).
5. Push to the branch (`git push origin my-feature-branch`).
6. Open a pull request.

## Future Improvements (TODO)

The library can be extended with additional features in future updates:
- [ ] Log file output and rotation.
- [ ] Configurable log levels (e.g., disable debug logs in production).
- [ ] Asynchronous logging to improve performance.
- [ ] Support for structured logging formats like JSON.

## License

`clog` is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
