# Makefile

# Step 1: Define directories
BUILD_DIR := build
SRC_DIR := src
INC_DIR := include
LINKER_DIR := linker
EXT_DIR := external

# Step 2: Import files
SOURCES := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
STARTUP_SOURCE := $(wildcard $(SRC_DIR)/*/*.S)
EXT_SOURCES := $(wildcard $(EXT_DIR)/*/*.c)
INCLUDES := -I$(INC_DIR) -I$(EXT_DIR)/printf -I$(INC_DIR)/motor -I$(INC_DIR)/lcd -I$(INC_DIR)/temp

# Step 3: create name of the final target
TARGET := main

# Step 4: Setup flags based on platform
CC := arm-none-eabi-gcc
LINKER_FILE := -T $(LINKER_DIR)/stm32f446xe_flash.ld
CPU := -mcpu=cortex-m4
ARCH_FLAGS := -mthumb \
							-mfloat-abi=hard \
							-mfpu=fpv4-sp-d16 \
							--specs=nosys.specs
CPPFLAGS :=
LDFLAGS := $(LINKER_FILE) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size

WFLAGS = -Wall -Wpedantic -Wextra -Wshadow -Werror

# Note: Compile with debug optimisation and debug flags enabled
CFLAGS := $(WFLAGS) -std=c99 -g -Og

# Step 5: Create names for the files
# Note: Substitute the object files with the same name as the sources with a different extension (input, replacement, the actual text)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
STARTUP_OBJECTS := $(patsubst $(SRC_DIR)/%.S,$(BUILD_DIR)/%.o,$(STARTUP_SOURCE))
EXT_OBJECTS := $(patsubst $(EXT_DIR)/$.c,$(BUILD_DIR)/%.o,$(EXT_SOURCES))
DEPS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.d,$(SOURCES))

# Note: Read header file dependencies when available
-include $(DEPS)

# Step 6: Create recipes
# Note: 
# - Create directory by extracting the directory portion of the path
# - This recipe is to create assembly for a single source file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(CPU) $(ARCH_FLAGS) $(INCLUDES) -MMD -MP -MF $(BUILD_DIR)/$*.d -c $< -o $@

# Build the board startup assembly file into an object
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(CPU) $(ARCH_FLAGS) -c $< -o $@

# Step 7: Create rules
.PHONY: compile-all
compile-all: $(OBJECTS)

.PHONY: build
build: $(BUILD_DIR)/$(TARGET).out
$(BUILD_DIR)/$(TARGET).out: $(OBJECTS) $(STARTUP_OBJECTS) $(EXT_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(CPU) $(ARCH_FLAGS) $(INCLUDES) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

# Note: This is to create assembly for the final linked binary by disassembling the it and showing the original C code alongside the assembly
.PHONY: $(TARGET).asm
$(TARGET).asm: $(BUILD_DIR)/$(TARGET).out
	$(OBJDUMP) -S $< > $(BUILD_DIR)/$@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Run should be dependent on the final target
.PHONY: run
run: $(BUILD_DIR)/$(TARGET).out
	./$<
