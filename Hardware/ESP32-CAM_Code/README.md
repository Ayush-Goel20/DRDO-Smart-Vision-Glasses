
### 2. Hardware/ESP32-CAM_Code/README.md

```markdown
# ESP32-CAM Code for Smart Vision Glasses

This directory contains the Arduino code for the ESP32-CAM module used in the Smart Vision Glasses.

## Components Used
- ESP32-CAM module
- MicroSD card
- 5V rechargeable battery
- Speaker for audio output

## Setup Instructions
1. Install Arduino IDE
2. Add ESP32 board support
3. Install required libraries:
   - ESP32 Camera library
   - SD library
4. Connect ESP32-CAM to computer
5. Upload the code

## Pin Configuration
- Camera: OV2640
- SD Card: Connected to standard SPI pins
- Speaker: Connected to GPIO XX

## Functionality
- Captures images from camera
- Processes images for object detection
- Provides audio output through connected speaker
- Stores logs on SD card
