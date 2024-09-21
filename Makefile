# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pthread -Isrc

# Directories
SRC_DIR = ./src
TESTS_DIR = ./tests
BUILD_DIR = ./build

# Source files
SRC_FILES = $(SRC_DIR)/clog.c

# Test source files
TEST_FILES = $(wildcard $(TESTS_DIR)/*.c)

# Object files for each test
OBJ_FILES = $(patsubst $(TESTS_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_FILES))

# Test executables
TEST_EXECUTABLES = $(patsubst $(TESTS_DIR)/%.c, $(BUILD_DIR)/%, $(TEST_FILES))

# Targets
all: $(BUILD_DIR) $(TEST_EXECUTABLES)

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile each test file into an executable
$(BUILD_DIR)/%: $(TESTS_DIR)/%.c $(SRC_FILES)
	$(CC) $(CFLAGS) $< $(SRC_FILES) -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR)

# Run all tests
test: all
	@for test in $(TEST_EXECUTABLES); do \
		echo "Running $$test..."; \
		./$$test || exit 1; \
	done

.PHONY: all clean test
