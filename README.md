# Automatic Trash Bin
## Project Description
This project is an implementation of an automatic trash can designed to open and close the lid automatically using a microcontroller. This project integrates various concepts from the Microprocessors and Interfaces course, including the use of registers, ADC (Analog-to-Digital Converter), PWM (Pulse Width Modulation), timers, interrupts, and serial communication.

## Main Features
- **Ultrasonic Sensor**: Detects the presence of the user's hand to open the trash can lid.
- **Servo Control**: Uses PWM to control the servo motor that opens and closes the trash can lid.
- **Status Indicator**: Uses RGB LEDs to show the status of the bin (whether it is full or not).
- **Serial Communication**: Displays status information on the serial monitor.

## Components Used
- Arduino Uno
- Ultrasonic Sensor (HC-SR04)
- Servo Motor (SG90)
- RGB LEDs
- Battery
- Jumper Cables

## Installation
1. Clone this repository to your computer.
2. Open the `AutomaticTrashBin.ino` file using the Arduino IDE.
3. Connect the components according to the circuit schematic provided.
4. Upload the code to Arduino Uno.
5. Open Serial Monitor to view the bin status.

## How to use
- Place your hand in front of the ultrasonic sensor to open the dustbin lid.
- The RGB LED will light green if the trash can is still usable and red if it is full.
