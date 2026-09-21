# Makefile

include sources.mk

# Step 3: create name of the final target
TARGET := main

# Step 4: Setup flags based on platform
CC := arm-none-eabi-gcc
LINKER_FLASH := -T $(LINKER_DIR)/STM32F446RETX_FLASH.ld
CPU := -mcpu=cortex-m4
ARCH_FLAGS := -mthumb \
							-mfloat-abi=hard \
							-mfpu=fpv4-sp-d16 \
							-ffunction-sections \
							-fdata-sections \
							-fstack-usage \
							--specs=nano.specs

CPPFLAGS :=
LDFLAGS := $(LINKER_FLASH) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size

WFLAGS = -Wall -Wpedantic -Wextra -Wshadow -Werror

# Note: Compile with maximum debug info and zero optimisation
CFLAGS := $(WFLAGS) -std=gnu11 -g3 -O0 -DSTM32F446xx

# Step 5: Create names for the files
# Note: Substitute the object files with the same name as the sources with a different extension (input, replacement, the actual text)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
STARTUP_OBJECTS := $(patsubst $(SRC_DIR)/%.S,$(BUILD_DIR)/%.o,$(STARTUP_SOURCE))
# EXT_OBJECTS := $(patsubst $(EXT_DIR)/$.c,$(BUILD_DIR)/%.o,$(EXT_SOURCES))
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
