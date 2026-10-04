# Initial GDB Commands to connect to ST-Link Debugger and launch the GUI 

target extended-remote localhost:3333
monitor reset halt
load
break main
tui enable
continue
