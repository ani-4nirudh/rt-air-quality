# sources.mk

# Step 1: Define directories
BUILD_DIR := build
COMMON_DIR := common

CORE_DIR := src
LINKER_DIR := linker

# Step 2: Import files
SOURCES := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
STARTUP_SOURCE := $(wildcard $(SRC_DIR)/*/*.S)
INCLUDES := -I./cmsis -I./config -I./core/include -I./middleware/FreeRTOS/Sourc
