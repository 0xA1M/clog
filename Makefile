# Detect platform
ifeq ($(OS),Windows_NT)
  CC        := mingw32-gcc
  LDFLAGS   :=
  CLEAN_CMD := del /Q
else
  CC        := gcc
  LDFLAGS   := -pthread
  CLEAN_CMD := rm -rf
endif

# Compiler Flags
CFLAGS    := -Wall -Wextra -Wno-trigraphs  -I.

# Directories
SRC_DIR   := tests
BUILD_DIR := build

# Test driver and implementations
# tests/test_runner.c contains the main() function
TEST_MAIN    := $(SRC_DIR)/runner.c
# All other test implementation files
TEST_IMPLS   := $(filter-out $(TEST_MAIN), $(wildcard $(SRC_DIR)/*.c))

.PHONY: all tests clean

all: tests

tests: $(BUILD_DIR)/test_suite
	@echo "Built test suite: $(BUILD_DIR)/test_suite"

# Link main with all test implementations
$(BUILD_DIR)/test_suite: $(TEST_MAIN) $(TEST_IMPLS) clog.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(CLEAN_CMD) $(BUILD_DIR)/
