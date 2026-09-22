# sources.mk

# Define directories
BUILD_DIR := build
COMMON_DIR := common
SRC_DIR := core/source
LINKER_DIR := linker
STARTUP_DIR := startup
FREERTOS_DIR := middleware/FreeRTOS
PERI_DIR := peripherals

# Add filepath for the startup file
STARTUP_SOURCE := $(wildcard $(STARTUP_DIR)/*.s)

# Add filepaths
FREERTOS_SOURCES := $(wildcard $(FREERTOS_DIR)/Source/*.c \
															 $(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F/*.c \
															 $(FREERTOS_DIR)/Source/portable/MemMang/*.c)

# Add filepaths for peripherals
PERI_SOURCES := $(wildcard $(PERI_DIR)/source/*.c)

# Add them together as the files have the same '.c' extension
SOURCES := $(wildcard $(SRC_DIR)/*.c)
SOURCES += $(FREERTOS_SOURCES) $(PERI_SOURCES)

# Add include files
INCLUDES := -I./cmsis \
						-I./config \
						-I./core/include \
						-I./core/source \
						-I./peripherals/include \
						-I$(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F \
						-I$(FREERTOS_DIR)/Source/include/ 
