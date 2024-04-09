# Created by TuanTNT19

## Description
This project introduces a snake game designed for the BeagleBone Black, utilizing an SSD1306 OLED screen. The game's version control is managed through a button interface.

## Hardware Configuration
- **SSD1306_SCL:** Connected to P9_19
- **SSD1306_SDA:** Connected to P9_20
- **SSD1306_GND:** Connected to P9_01 
- **SSD1306_VCC:** Connected to P9_07
- **Up Button:** Connected to P9_02 and P8_13
- **Down Button:** Connected to P9_02 and P8_10
- **Left Button:** Connected to P9_02 and P8_12
- **Right Button:** Connected to P9_02 and P8_11
- **Enter Button:** Connected to P9_02 and P8_09

## Directory Structure
- `bin`: Directory for the executable files
- `obj`: Directory for the object files
- `inc`: Directory for the header files
- `src`: Directory for the source files
- `lib`: Directory for the static and shared libraries
- `button driver`: Directory for driver button and make file
- `ssd1306 driver`: Directory for driver ssd1306, and make file

## Usage Instructions
### On Ubuntu:
- In `./button driver`, execute `make` to construct the `driver_button.ko` file and transfer it to the BeagleBone.
- In `./ssd1306 driver`, execute `make` to build `ssd1306_oled_driver.ko` and transfer it to the BeagleBone.
- In `.`:
  - For using the static library: Execute `make sta_all` to build the executable file named `main` in `bin/` and send it to the BeagleBone.
  - For using the shared library: Execute `sudo make share_all` to construct the `main` executable file in `bin/` and the shared library named `libsnake_game_button.so`. Transfer both files to the BeagleBone.

### On BeagleBone:
- Run `sudo insmod driver_button.ko` to insert the button module into the kernel.
- Run `sudo insmod ssd1306_oled_driver.ko` to insert the SSD1306 module into the kernel.
- If using the static library in Ubuntu: Execute `./main` to launch the game.
- If using the shared library in Ubuntu:
  - Copy the `libsnake_game_button.so` to `/usr/lib` by executing `cp -rf ./libsnake_game_button.so /usr/lib`.
  - Execute `./main` to launch the game.
  
## Video Demo
[Watch the video demo](https://youtu.be/gbpuqwJCehI)
