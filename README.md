This project implements a flight controller for a drone using ESP-IDF (Espressif IoT Development Framework). The flight controller utilizes data from a Motion Capture (MOCAP) system to obtain roll, pitch, and yaw information of the drone, ensuring precise control and stability during flight.

Features:
Motion Capture Integration: Utilizes data from a Motion Capture system to accurately determine the orientation of the drone in terms of roll, pitch, and yaw angles.

IMU for Fail-Safe: Incorporates an Inertial Measurement Unit (IMU) for fail-safe operation, providing redundancy in case of loss of Motion Capture data or other failures.

Lidar for Altitude Control: Integrates a Lidar sensor to measure the distance from the ground, enabling precise altitude control and obstacle avoidance.

ESP-IDF Framework: Built using the ESP-IDF framework, leveraging the power and flexibility of Espressif's development platform for embedded applications.

Usage:
Setup ESP-IDF Environment: Make sure you have set up the ESP-IDF development environment according to the official documentation.

Clone the Repository: Clone this repository to your local machine.

Configure Project: Modify the configuration files as necessary to match your hardware setup, including Motion Capture system, IMU, and Lidar sensor configurations.

Build and Flash: Build the project using idf.py build and flash it to your ESP32 device using idf.py -p [PORT] flash.

Run: Power on your drone with the ESP32 device running the flight controller firmware. Ensure that the Motion Capture system is properly configured and providing data to the flight controller.

Monitor and Test: Monitor the drone's behavior using a ground control station or telemetry system. Test various flight maneuvers to ensure stability and performance.

Contributions:
Contributions to this project are welcome! If you find any bugs or have ideas for improvements, feel free to open an issue or submit a pull request.

License:
This project is licensed under the MIT License.
