# Eco-Cooling Smart Desk Fan 🌬️🌡️

An intelligent, energy-saving desk fan controller based on Arduino. The system uses a combination of presence detection and contactless temperature measurement to ensure the fan only operates when a user is at the desk and the temperature exceeds a comfortable threshold.

## 🚀 Features
* **Smart Activation:** The fan turns on only if motion is detected AND the object temperature is above the defined threshold (default: 26.0°C).
* **Contactless Temperature Reading:** Uses the MLX90614 IR sensor via the I2C protocol to read object temperature without physical contact.
* **PWM Soft-Start & Soft-Stop:** Implements Pulse Width Modulation (PWM) to slowly ramp up and ramp down the DC motor, preventing sudden voltage spikes and reducing mechanical wear.
* **Status Indication:** A built-in LED provides visual feedback on the system's current state (standby/active).

## 🛠️ Hardware Requirements
* **Microcontroller:** Arduino Uno (or compatible)
* **Temperature Sensor:** MLX90614 (Contactless IR Thermometer)
* **Motion Sensor:** HC-SR505 (Mini PIR)
* **Actuator:** 5V DC Fan with a transistor/relay module
* **Indicator:** Standard LED with an appropriate resistor (e.g., 220Ω)
* **Misc:** Jumper wires, breadboard

## 🔌 Pinout & Wiring

| Component       | Arduino Pin | Details / Protocol       |
|-----------------|-------------|--------------------------|
| MLX90614 (SDA)  | A4 (or SDA) | I2C Data Line            |
| MLX90614 (SCL)  | A5 (or SCL) | I2C Clock Line           |
| HC-SR505 (OUT)  | D8          | Digital Input            |
| Fan Module (IN) | D3          | Digital Output (PWM)     |
| Status LED      | D9          | Digital Output           |

*Note: Always connect the VCC and GND pins of the sensors and modules to the respective 5V and GND pins on the Arduino.*

## 💻 Software & Libraries
To compile and upload this code, you need the Arduino IDE and the following libraries:
1. `Wire.h` (Built-in Arduino library for I2C communication)
2. `Adafruit_MLX90614` (Can be installed via the Arduino Library Manager)

## ⚙️ How it works (Logic Flow)
1. **Initialization:** The system configures pins and attempts to establish an I2C handshake with the MLX90614 sensor. If the sensor is unresponsive, the system halts to prevent erroneous operation.
2. **Standby Mode:** The system blinks the status LED and polls the PIR sensor. The fan remains off to save power.
3. **Active Mode:** Upon detecting motion, the status LED stays on, and the system queries the MLX90614 sensor for the object temperature.
4. **Cooling Cycle:** If the temperature > 26.0°C, the fan smoothly accelerates to 100% power for a 10-second cooling cycle, after which it smoothly decelerates. 

## 🔮 Future Improvements (Roadmap)
* **Non-blocking Delays:** Refactor the codebase to replace `delay()` with `millis()`, allowing for real-time sensor polling without thread-blocking.
* **IoT Integration:** Forward UART serial data (temperature, presence status) to a backend service (e.g., Python + PostgreSQL running in a Docker container) for data logging and analytics.
* **State Machine:** Implement a robust finite state machine (FSM) to handle transitional states more elegantly.

---
*Developed as a hardware-level R&D project focusing on Embedded C++, sensor integration, and I2C communication.*