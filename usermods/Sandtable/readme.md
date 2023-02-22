# Sand table

This mod connects zu FluidNC via Serial Port and gets its SD card's or internal file system's content.
After that it makes FluidNC run one file after another automatically.


## Commands of interest

[List of commands](https://github.com/bdring/FluidNC/wiki/FluidNC-Commands-and-Settings)

| Command               | Description   |
|-----------------------|---------------|
| `$State`              | Shows the current state (Idle, Run, Jog, etc) |
| `$Home`               | Homes all axes as set up in config file |
| `$Alarm/Disable`      | Tries to clear the alarm state |
| `$System/Sleep`       | Lets the system sleep |
| `$Limits/Show`        | Enters a mode where the state of the limit switch inputs are shown. This can be used to test the connections. Send ! to exit this mode. |
| `$Bye`                | Reboots the processor |
| `$Motor/Disable`      | Disable all motors    |
| `$System/Control`     | *This command needs further investigation* |
| `$System/Stats`       | *This command needs further investigation* |
| `$Notification/Send`  | *This command needs further investigation* |
| `$Notification/Setup` | *This command needs further investigation* |
| [`$SD/Status`](https://github.com/bdring/FluidNC/wiki/SD-Card#sdstatus---get-sd-card-status) | Gets the SD card's current state |
| [`$SD/List`](https://github.com/bdring/FluidNC/wiki/SD-Card#sdlist---get-sd-card-content) | List all files on the SD card (incl. all subdirectories) |
| [`$SD/Run`](https://github.com/bdring/FluidNC/wiki/SD-Card#sdlist---get-sd-card-content) | Runs the specified file |
| [`$LocalFS/List`](https://github.com/bdring/FluidNC/wiki/Local-File-System#listing-files) | List all files of the local file system |
| [`$LocalFS/Run`](https://github.com/bdring/FluidNC/wiki/Local-File-System#running-files) | Runs the specified file |
| [`$Jog`](https://github.com/gnea/grbl/wiki/Grbl-v1.1-Jogging) | Could be interested for pong controller style drawing mode |


## Installation (from EXAMPLE_v2)

Copy `usermod_v2_example.h` to the wled00 directory.  
Uncomment the corresponding lines in `usermods_list.cpp` and compile!  
_(You shouldn't need to actually install this, it does nothing useful)_

