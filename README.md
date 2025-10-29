ADAPTIVE CLIMATE CONTROL SYSTEM

The Adaptive Climate Control System is a smart Arduino-based project designed to automatically maintain comfortable indoor temperature using ambient light and temperature data.
It provides an intelligent and energy-efficient approach to climate control by adjusting fan operation based on surrounding light intensity and temperature levels.


INTRODUCTION

This project adapts to environmental changes by dynamically controlling a fan using feedback from sensors.
A photoresistor (LDR) detects ambient light to adjust the temperature setpoint — brighter light means a lower set temperature, while dimmer light increases the set temperature.
A DHT11 temperature sensor measures the actual room temperature, and a PID control algorithm calculates the error between the set and actual temperatures.

When the actual temperature exceeds the set temperature, the fan turns ON to cool the environment.

When the set temperature is higher than the actual, the fan remains OFF to save power.

A 16x2 LCD displays the Set Temperature, Actual Temperature, and Fan Status in real time.

This system demonstrates the integration of sensors, actuators, and control algorithms for adaptive home automation.


COMPONENTS AND SPECIFICATIONS
| Component                     | Description                                            | Specification             |
| ----------------------------- | ------------------------------------------------------ | ------------------------- |
| **Microcontroller**           | Main processing and control unit                       | **Arduino Uno**           |
| **Light Sensor (LDR)**        | Detects light intensity to adjust temperature setpoint | **10 kΩ Photoresistor**   |
| **Temperature Sensor**        | Measures real-time room temperature                    | **DHT11**                 |
| **DC Motor (Fan)**            | Provides cooling based on control signal               | **5V DC Motor**           |
| **Motor Driver / Transistor** | Interface between Arduino and motor                    | **L293D / BC547**         |
| **LCD Display**               | Shows Set Temp, Actual Temp, and Fan status            | **16x2 LCD (I2C)**        |
| **Resistor**                  | Used in voltage divider circuit with LDR               | **10 kΩ fixed resistor**  |
| **Power Supply**              | Powers the Arduino and components                      | **5V DC (USB / Adapter)** |
| **Breadboard & Jumper Wires** | For prototyping connections                            | **Standard**              |



SYSTEM WORKFLOW

LDR senses ambient light and adjusts the temperature setpoint accordingly.

DHT11 continuously measures the actual temperature.

The Arduino calculates the error using:
error = setTemperature - actualTemperature

A PID controller processes this error and determines fan control output.

The DC motor (fan) is activated if actual temperature > set temperature.

The LCD updates to show current values and fan status.
