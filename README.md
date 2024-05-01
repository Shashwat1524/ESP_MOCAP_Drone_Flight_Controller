# Drone Flight Controller using ESP-IDF and PhaseSpace Motion Capture System

This project implements a flight controller for a drone using ESP-IDF (Espressif IoT Development Framework). The flight controller utilizes data from a Motion Capture (MOCAP) system, specifically the PhaseSpace Motion Capture System, to obtain roll, pitch, and yaw information of the drone through rigids.

## Installation
### ESP-IDF
Install ESP-IDF by following the instructions in the Espressif IoT Development Framework (ESP-IDF) documentation.
https://docs.espressif.com/projects/esp-idf/en/stable/esp32/index.html 

### ROS2 Humble
Install ROS2 Humble. You can find installation instructions here. 
https://docs.ros.org/en/humble/Installation.html

**Cloning micro-ROS Repository**
Navigate to the components folder in your project directory.
Clone the micro-ROS repository by executing the following command:
```bash
git clone https://github.com/micro-ROS/micro_ros_espidf_component
```
Note: This repository is not owned by the author of this project.
Setting up micro-ROS
Once the micro-ROS repository is cloned, perform the following steps:

```bash
idf.py set-target esp32
idf.py menuconfig
```
In the menu configuration, set your micro-ROS configuration and WiFi credentials under micro-ROS Settings.
Build the project using:
```bash
idf.py build
```
Flash the project onto your ESP32 board:
```bash
idf.py flash
```
OR
```bash
idf.py -p <PORT> flash 
```
Monitor the output to observe the flight controller behavior:
```bash
idf.py monitor
```
## Additional Notes
Ensure that your ESP32 board is properly connected and configured before flashing the project.
Make sure that the PhaseSpace Motion Capture System is properly calibrated and providing accurate data before integrating it with the flight controller.
