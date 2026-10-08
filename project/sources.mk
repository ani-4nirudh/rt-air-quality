# sources.mk

# Define directories
BUILD_DIR := build
CMSIS_DIR := cmsis
COMMON_DIR := common
CONFIG_DIR := config
CORE_DIR := core
FREERTOS_DIR := middleware/FreeRTOS
LINKER_DIR := linker
LOG_DIR := logs
PERI_DIR := peripherals
STARTUP_DIR := startup
TASKS_DIR := app_tasks

# Define filenames
GDB_INIT := .gdbinit

# Add filepath for the startup file
STARTUP_SOURCE := $(wildcard $(STARTUP_DIR)/*.s)

# Add filepaths
FREERTOS_SOURCES := $(wildcard $(FREERTOS_DIR)/Source/*.c \
															 $(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F/*.c \
															 $(FREERTOS_DIR)/Source/portable/MemMang/*.c)

# Add filepaths for peripherals
PERI_SOURCES := $(wildcard $(PERI_DIR)/source/*.c)

# Add them together as the files have the same '.c' extension
SOURCES := $(wildcard $(CORE_DIR)/source/*.c)
SOURCES += $(FREERTOS_SOURCES) $(PERI_SOURCES)

# Add include files
INCLUDES := -I./$(CMSIS_DIR) \
						-I./$(COMMON_DIR)/include \
						-I./$(CONFIG_DIR) \
						-I./$(CORE_DIR)/include \
						-I./$(CORE_DIR)/source \
						-I./$(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F \
						-I./$(FREERTOS_DIR)/Source/include \
						-I./$(PERI_DIR)/include \
						-I./$(PERI_DIR)/source \
						-I./$(TASKS_DIR)/include
