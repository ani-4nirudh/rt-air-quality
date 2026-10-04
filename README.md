# Real-time Air Quality Sensor (In Progress)
**WIP**
*Please wait for the final instructions.*
## Overview

## Build Instructions
```
# Install the OpenOCD tool
sudo apt install openocd

# Compile and build the final executable
cd project
make build
```

## Debug Instructions
```
make debug

# Open a new terminal

# Launch the executable and setup the remote connection with the board
arm-none-eabi-gdb build/main.out
target extended-remote localhost: 3333

# gdb commands
monitor reset halt
load
breakpoint main
tui enable
continue
```

## Hardware


## Architecture
- System diagrams

### Additional Documentation
- Hardware
- Pinout

