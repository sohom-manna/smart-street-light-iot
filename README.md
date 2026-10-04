# Smart Street Light System

An IoT-enabled smart street lighting prototype using an ESP8266 NodeMCU, LDR sensor, IR motion sensors, LEDs, and ThingSpeak for remote monitoring.

## 📌 Overview

Conventional street lighting can consume significant energy when lights remain active even when there is sufficient ambient light or no movement.

This project demonstrates an automatic street lighting system that uses an **LDR sensor for ambient-light detection** and **three IR sensors for motion detection**. The ESP8266 NodeMCU processes the sensor inputs, controls the corresponding street-light LEDs, and sends sensor data to the **ThingSpeak cloud platform** over Wi-Fi.

The prototype was designed and tested as an academic project in Electronics & Communication Engineering.

## ✨ Key Features

- Automatic day/night detection using an LDR
- Motion detection using three IR sensors
- Individual control of three street-light LEDs
- ESP8266 Wi-Fi connectivity
- Relay-based power control
- Automatic LED timeout after motion detection
- Remote sensor monitoring through ThingSpeak
- Real-time LDR and IR sensor data logging

## 🧰 Hardware

| Component | Purpose |
|---|---|
| ESP8266 NodeMCU | Main microcontroller and Wi-Fi connectivity |
| LDR Sensor | Ambient-light / day-night detection |
| 3 × IR Sensors | Motion / obstacle detection |
| 3 × LEDs | Street-light prototype |
| Relay Module | Controls the street-light supply |
| Resistors | Current limiting and sensor circuit |
| Breadboard | Prototype assembly |
| Jumper Wires | Electrical connections |
| 5V Power Supply | System power |

## 💻 Software & Platforms

- **Arduino IDE**
- **C/C++**
- **ESP8266WiFi library**
- **ThingSpeak library**
- **ThingSpeak Cloud**
- Wi-Fi communication using ESP8266

## 🔌 Pin Configuration

| Function | ESP8266 Pin |
|---|---|
| LDR | A0 |
| IR Sensor 1 | D5 |
| IR Sensor 2 | D6 |
| IR Sensor 3 | D7 |
| LED 1 | D1 |
| LED 2 | D2 |
| LED 3 | D4 |
| Relay | D3 |

## 🧠 Working Principle

The system operates using two main sensing stages:

### 1. Ambient Light Detection

The LDR is connected as a voltage-divider circuit and its analog value is read through the ESP8266 ADC.

The controller compares the measured LDR value with a predefined threshold to determine whether the system should operate in day mode or night mode.

### 2. Motion Detection

During night operation, the three IR sensors continuously monitor their respective areas.

When an IR sensor detects an object:

- The corresponding LED is activated.
- The relay is activated when required.
- The LED remains active for a predefined period.
- The LED is automatically switched OFF after the timeout if no further detection occurs.

### 3. IoT Monitoring

The ESP8266 connects to a Wi-Fi network and periodically uploads:

- LDR value
- IR Sensor 1 value
- IR Sensor 2 value
- IR Sensor 3 value

to ThingSpeak.

This allows the sensor readings and system activity to be monitored remotely.

## 🔄 System Flow

```text
        Ambient Light
             │
             ▼
          ┌───────┐
          │  LDR  │
          └───┬───┘
              │
              ▼
       ┌──────────────┐
       │   ESP8266    │
       │   NodeMCU    │
       └──────┬───────┘
              │
       ┌──────┴───────┐
       │              │
       ▼              ▼
  IR Sensors      Wi-Fi / IoT
  IR1 IR2 IR3         │
       │              ▼
       ▼         ThingSpeak
   LED Control
       │
       ▼
 Street Light
