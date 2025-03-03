# Emergency Drive Design

**Making and Hacking 2023-24 (Group 11)**

This repository showcases our university project for designing an **Emergency Drive Mechanism**. It integrates CAD designs, Arduino code, and an Android application to create a functional prototype.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Project Structure](#project-structure)
- [Installation and Usage](#installation-and-usage)
  - [CAD Files](#cad-files)
  - [Arduino Code](#arduino-code)
  - [Android App](#android-app)
- [License](#license)

---

## Introduction
The project addresses the challenge of designing an **emergency actuation mechanism** using:
- **Interchangeable cams** and followers.
- A **functional Android app** for control.
- Arduino-based **motor and sensor integration**.

The mechanism ensures directional rotation with minimal switch-over time and simulates the mechanism of an old tank.

---

## Features
- **Mechanical Design:** Detailed CAD models of the cam-follower system and supporting structure.
- **Electronics Integration:** Arduino-based motor control and sensor feedback.
- **Mobile Application:** Android app for real-time control and monitoring.

---

## Project Structure
```plaintext
EmergencyDriveDesign/
├── CAD_Files/      # 3D models and technical drawings
├── Arduino_Code/   # Source code for Arduino microcontroller
├── Android_App/    # Android app (integrated as a Git subtree)
├── README.md       # Project overview
└── LICENSE         # Licensing information
```

---

## Installation and Usage

### CAD Files
1. Use a 3D modeling tool like [Creo Parametric](https://www.ptc.com/en/products/creo).
2. Open files from the `CAD_Files/` directory to explore or modify the mechanical components.

### Arduino Code
1. Install the Arduino IDE from [here](https://www.arduino.cc/en/software).
2. Connect your Arduino board and upload the code from the `Arduino_Code/` directory.
3. Components include:
   - **Motor**: Controlled via PWM for precise actuation.
   - **Sensors**: Rotary encoder for position tracking.

### Android App
The Android application is included in this repository as a **Git subtree**.

1. **Cloning & Initialization:**
   - When cloning the main repository, use:
     ```bash
     git clone --recurse-submodules http://github.com/jjaroztegi/Emergency-Cam-System.git
     ```
     If you already cloned without submodules, initialize the subtree with:
     ```bash
     git subtree update --init --recursive --prefix=Android_App
     ```

2. **Opening & Building:**
   - Open the `Android_App/` directory in Android Studio.
   - Build and run the app on your device to control the drive mechanism.

3. **Updating the Subtree:**
   - To pull the latest changes from the Android app’s original repository, run:
     ```bash
     git subtree pull --prefix=Android_App https://github.com/jjaroztegi/Emergency-Cam-System-Android.git main --squash
     ```

---

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

**Disclaimer**: This project was created for educational purposes and may require additional customization for specific use cases.
