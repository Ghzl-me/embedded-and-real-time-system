Traffic Light Control System

Overview

This project is a two-direction traffic light control system designed and simulated using an Arduino Uno.

The system controls traffic lights for two intersecting roads (North-South and East-West). It also allows users to adjust the green light duration using push buttons and displays the timing information on a 16x2 I2C LCD.

Features

- Automatic traffic light control for two directions.
- Six LEDs representing red, yellow, and green traffic lights.
- Adjustable green light duration using push buttons.
- Real-time countdown displayed on an LCD.
- Safe transition between directions using an all-red phase.
- Non-blocking timing using "millis()".
- Button debouncing to prevent unwanted multiple inputs.

Hardware Components

- Arduino Uno
- 6 LEDs (2 red, 2 yellow, 2 green)
- 6 × 220Ω resistors
- 4 push buttons
- 16x2 I2C LCD
- Jumper wires

Pin Configuration

Component| Arduino Pin
North-South Red LED| 8
North-South Yellow LED| 9
North-South Green LED| 10
East-West Red LED| 11
East-West Yellow LED| 12
East-West Green LED| 13
North-South Increase Button| 2
North-South Decrease Button| 3
East-West Increase Button| 4
East-West Decrease Button| 5
LCD SDA| A4
LCD SCL| A5

The LCD uses I2C address "0x27".

Traffic Light Sequence

The system operates through six states:

1. North-South Green
2. North-South Yellow
3. All Red (Safety Interval)
4. East-West Green
5. East-West Yellow
6. All Red (Safety Interval)

After the final state, the sequence repeats automatically.

Timing Configuration

Parameter| Default Duration
Green Light| 5000 ms
Yellow Light| 2000 ms
All-Red Interval| 1000 ms
Button Adjustment Step| 1000 ms
Button Debounce| 200 ms

The green light duration can be increased or decreased independently for each direction.

LCD Display

The 16x2 I2C LCD displays:

- Green light duration for each direction.
- Remaining time of the current traffic light phase.

Software Requirements

- Arduino IDE
- "LiquidCrystal_I2C" library
- Wokwi Arduino Simulator (optional)

How to Run

1. Open "sketch.ino" in Arduino IDE or Wokwi.
2. Install the "LiquidCrystal_I2C" library if it is not already installed.
3. Connect the components according to the pin configuration.
4. Upload the sketch to the Arduino Uno or start the Wokwi simulation.
5. Use the push buttons to adjust the green light durations.

Project Files

- "sketch.ino" — Arduino source code.
- "diagram.json" — Wokwi circuit configuration.

Educational Purpose

This project demonstrates practical concepts in embedded systems, including digital input/output, finite state machines, time-based programming, button debouncing, and I2C communication.