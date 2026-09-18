# sources.mk

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
