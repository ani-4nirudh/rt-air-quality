# sources.mk

# Step 1: Define directories
BUILD_DIR := build
COMMON_DIR := common
SRC_DIR := ./core/source
LINKER_DIR := linker
STARTUP_DIR := startup
FREERTOS_DIR := ./middleware/FreeRTOS

# Step 2: Import files
FREERTOS_SOURCES := $(wildcard $(FREERTOS_DIR)/Source/*.c \
															 $(FREERTOS_DIR)/portable/GCC/ARM_CM4F/*.c \
															 $(FREERTOS_DIR)/portable/MemMang/*.c)
STARTUP_SOURCE := $(wildcard $(STARTUP_DIR)/*.s)
SOURCES := $(wildcard $(SRC_DIR)/*.c)

INCLUDES := -I./cmsis \
						-I./config \
						-I./core/include \
						-I./core/source \
						-I$(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F \
						-I$(FREERTOS_DIR)/Source/include/ 
