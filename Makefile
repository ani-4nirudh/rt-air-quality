# Makefile

# Step 1: Define directories
BUILD_DIR := build
SRC_DIR := src
INC_DIR := include

# Step 2: Import files
SOURCES := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
INCLUDES := -I$(INC_DIR)/

# Step 3: create name of the final target
TARGET := main

# Step 4: Setup flags
CC := gcc
WFLAGS = -Wall -Wpedantic -Wextra -Wshadow -Werror
CPPFLAGS :=

# Note: Compile with debug optimisation and debug flags enabled
CFLAGS := $(WFLAGS) -std=c99 -g -Og

# Step 5: Create names for the files
# Note: Substitute the object files with the same name as the sources with a different extension (input, replacement, the actual text)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.d,$(SOURCES))

# Note: Read header file dependencies when available
-include $(DEPS)

# Step 6: Create recipes
# Note: This recipe is to create assembly for a single source file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) -MMD -MP -MF $(BUILD_DIR)/$*.d -c $< -o $@

# Step 7: Create rules
.PHONY: compile-all
compile-all: $(OBJECTS)

.PHONY: build
build: $(BUILD_DIR)/$(TARGET).out
$(BUILD_DIR)/$(TARGET).out: $(OBJECTS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) $^ -o $@
	size $@

# Note: This is to create assembly for the final linked binary by disassembling the it and showing the original C code alongside the assembly
.PHONY: $(TARGET).asm
$(TARGET).asm: $(BUILD_DIR)/$(TARGET).out
	objdump -S $< > $(BUILD_DIR)/$@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*

.PHONY: run
run:
	./build/main.out
