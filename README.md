PROJECT TITLE

Adaptive Climate Control System with PID and Ambient Light Feedback

This project implements a closed-loop control system using an Arduino to maintain the temperature of an enclosed environment. The desired temperature (Setpoint) is dynamically adjusted based on ambient light levels (simulating a comfort/energy-saving model). Cooling is achieved using a DC fan controlled by a finely tuned PID algorithm.

AUTHORS

Erum Iftikhar M01088213

Gayathri Satheesh L M01087828

PROBLEM STATEMENT 

The primary challenge is to create an energy-efficient and responsive climate control system that can stabilize the temperature of an enclosed space while adapting its target based on environmental context.

The main issue that was addressed was adaptive control, the system must vary the comfort level based on ambient light (LDR), setting a cooler target during high light and a warmer target during low light.

Variables:

Controlled Variable: Temperature (T_Actual in Degree Celcius)

Desired Variable: Adaptive Setpoint ($T_{Setpoint}$ in ${^\circ}C$), calculated from LDR input.

Disturbance(s): Ambient room temperature fluctuations, environmental heat (sunlight, adjacent electronics), and the simulated heat disturbance from the $5W, 10\Omega$ power resistor.
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
