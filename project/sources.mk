# sources.mk

# Define directories
BUILD_DIR := build
CMSIS_DIR := cmsis
COMMON_DIR := common
CORE_DIR := core
CONFIG_DIR := config
LINKER_DIR := linker
STARTUP_DIR := startup
FREERTOS_DIR := middleware/FreeRTOS
PERI_DIR := peripherals
LOG_DIR := logs
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
						-I ./$(COMMON_DIR)/include \
						-I./$(CONFIG_DIR) \
						-I./$(CORE_DIR)/include \
						-I./$(CORE_DIR)/source \
						-I./$(PERI_DIR)/include \
						-I./$(PERI_DIR)/source \
						-I$(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F \
						-I$(FREERTOS_DIR)/Source/include/ 
