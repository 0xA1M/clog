#include "logger.h"
#include <errno.h>
#include <stdio.h>

int main() {
    FILE *file = fopen("nonexistent_file.txt", "r");

    if (!file) {
        LOG(ERR, NULL, "Error opening file");
    }

    // Simulate another errno-based error
    errno = EACCES;
    LOG(WARN, NULL, "Simulated permission denied error");

    logger_cleanup();
    return 0;
}
