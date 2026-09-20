# sources.mk

# Step 1: Define directories
BUILD_DIR := build
COMMON_DIR := common
SRC_DIR := ./core/source
LINKER_DIR := linker
STARTUP_DIR := startup

# Step 2: Import files
SOURCES := $(wildcard $(SRC_DIR)/*.c)
STARTUP_SOURCE := $(wildcard $(STARTUP_DIR)/*.s)
INCLUDES := -I./cmsis \
						-I./config \
						-I./core/include \
						-I./core/source \
						-I./middleware/FreeRTOS/Source/portable/GCC/ARM_CM4F \
						-I./middleware/FreeRTOS/Source/include/ 
