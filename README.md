# DRDO Smart Vision Glass Project

A smart vision assistance system for visually impaired individuals, developed during an internship at Defence Research and Development Organization (DRDO), Dehradun.

## Overview

This project implements smart glasses that use computer vision and AI to detect objects in real-time and provide audio feedback to visually impaired users. The system consists of:

1. **Hardware Component**: ESP32-CAM based wearable glasses
2. **ML Object Detection**: Python-based YOLOv8 object detection
3. **Dashboard**: Streamlit web interface for visualization

## Project Structure

- `/Hardware` - Arduino code for ESP32-CAM
- `/Software/ML_Object_Detection` - Python implementation of YOLOv8 model
- `/Documentation` - Project report and related documents

## Team Members
- Ayush Goel
- Saloni Bhandari
- Anshika Kandari


## Mentors
- Dr. Vaibhav Gupta, Technical Officer 'C', DRDO Dehradun
- Dr. Anupam Singh, HOD-CSE, GEHU
- Dr. Vikrant Sharma, Assistant Professor, GEHU

## Institution
Graphic Era Hill University, Dehradun

## Getting Started

### Hardware Setup
1. Connect ESP32-CAM with required components
2. Upload the Arduino code from `/Hardware/ESP32-CAM_Code/`
3. Power the device using 5V battery

### Software Setup
For ML Object Detection:
```bash
cd Software/ML_Object_Detection
pip install -r requirements.txt
python main.py
