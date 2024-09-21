#include "logger.h"
#include <stdio.h>

int main() {
    // Test basic INFO log
    LOG(INFO, NULL, "This is an info message: %d", 42);

    // Test basic WARN log
    LOG(WARN, NULL, "This is a warning message: %s", "Potential Issue");

    // Test basic ERROR log
    LOG(ERR, NULL, "This is an error message");

    // Test basic DEBUG log
    LOG(DBG, NULL, "This is a debug message: %.2f", 3.14);

    // Clean up logger resources
    logger_cleanup();

    return 0;
}
