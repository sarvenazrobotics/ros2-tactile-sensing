# 🤖 Real-Time Tactile Sensing & Slip Detection Pipeline in ROS2

[![ROS2](https://img.shields.io/badge/ROS2-Jazzy-blue)](https://docs.ros.org/en/jazzy/)
[![C++](https://img.shields.io/badge/C%2B%2B-17-orange)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-Apache%202.0-green.svg)](https://opensource.org/licenses/Apache-2.0)
[![Status](https://img.shields.io/badge/Status-Active-success)]()

> A high-frequency (1000 Hz) tactile sensing and slip detection pipeline implemented in **C++** using **ROS2**, designed for real-time physical human-robot interaction and prosthetic hand applications.

---

## Overview

This project implements a complete real-time tactile perception pipeline for robotic manipulation. It features a custom ROS2 message architecture, a **1 kHz publisher node** simulating a 100-taxel tactile sensor array with optimized Quality of Service (QoS) settings, and a **real-time signal processing node** that computes force derivatives across all sensors to detect and flag slip events instantaneously.

The pipeline demonstrates core competencies in **real-time C++ execution**, **ROS2 middleware tuning**, **custom message design**, and **sensor-data integration** — directly addressing the perception and control requirements of modern human-robot interaction research.

This workspace bridges the gap between physical perception and kinematic modeling. It is designed as a foundational software stack for prosthetic hands and dexterous manipulators, featuring:
1. **Real-Time Perception**: A 1000 Hz C++ tactile sensor driver with optimized QoS and real-time slip detection via signal derivative calculation.
2. **Virtual Prototyping**: Parametric URDF/Xacro modeling of complex robotic hands (e.g., 16-DOF LEAP Hand), resolving Git LFS mesh dependencies for accurate 3D visualization.
3. **Real-Time Kinematics**: A custom C++ `tf2` node that asynchronously queries the TF tre

---




https://github.com/user-attachments/assets/4e9dd3d3-ccc1-4e04-ab7c-f703124c5a1d






---

##  Key Features

-  **1000 Hz real-time publishing** using `std::chrono` wall timers
-  **Custom ROS2 messages** for tactile arrays and slip events
-  **QoS tuning** with `best_effort` reliability for high-frequency sensor data
-  **Real-time signal processing** computing force derivatives across 100 taxels
-  **Threshold-based slip detection** with configurable sensitivity
-  **Clean package separation** between interfaces (`tactile_interfaces`) and nodes (`tactile_nodes`)
-  **Docker-ready** architecture for reproducible builds

---

##  Packages

### `tactile_interfaces`
Custom ROS2 message definitions:
- **`TactileArray.msg`**: High-frequency tactile sensor array (100 taxels) with timestamped header
- **`SlipEvent.msg`**: Slip detection event with derivative magnitude and boolean flag

### `tactile_nodes`
Two C++ ROS2 nodes:
1. **`tactile_sensor_driver`**: Simulates a 100-taxel tactile sensor publishing at 1000 Hz with `best_effort` QoS. Injects periodic 50N force spikes every 3 seconds to simulate object slip.
2. **`slip_detector`**: Subscribes to tactile data, computes real-time force derivatives ($dF/dt$), and publishes slip events when the derivative exceeds 1000 N/s.

---








## Installation & Build

### Prerequisites
- Ubuntu 22.04+ or 24.04
- ROS2 Humble or Jazzy
- C++17 compiler (GCC 11+ or Clang 14+)
- `colcon` build tool

### Build
```bash
# Clone the repository
git clone https://github.com/sarvenazrobotics/ros2-tactile-sensing.git
cd ros2-tactile-sensing

# Build the workspace
colcon build
# Source the workspace
source install/setup.bash
```
### Author 
Sarvenaz Ashoori
Italian Institute of Technology (IIT)
📧 sarvenaz.ashoori@iit.it




