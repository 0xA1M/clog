# Detect platform
ifeq ($(OS),Windows_NT)
    CC         := gcc
    LDFLAGS    :=
    RM         := rm -rf
    EXE        := .exe
else
    CC         := gcc
    LDFLAGS    := -pthread
    RM         := rm -rf
    EXE        :=
endif

# Compiler Flags
CFLAGS     := -Wall -Wextra -Wno-trigraphs -I.

# Directories
SRC_DIR    := tests
BUILD_DIR  := build

# Test driver and implementations
TEST_MAIN  := $(SRC_DIR)/runner.c
TEST_IMPLS := $(filter-out $(TEST_MAIN), $(wildcard $(SRC_DIR)/*.c))
TARGET     := $(BUILD_DIR)/test_suite$(EXE)

# Object files
OBJS       := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_MAIN) $(TEST_IMPLS))

.PHONY: all tests clean run

all: tests

tests: $(TARGET)
	@echo "✅ Built test suite: $(TARGET)"

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c clog.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Link test suite
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: tests
	@echo "🚀 Running test suite..."
	@$(TARGET)

clean:
	$(RM) $(BUILD_DIR)
