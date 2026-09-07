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

---
