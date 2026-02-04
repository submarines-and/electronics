# Serial monitor
Code to make an adafruit feather v2 act as a serial proxy for an attiny85. Using [ATtinySerialOut](https://github.com/ArminJo/ATtinySerialOut) on the attiny.

# Wiring
Connect 3v, gnd and feather pin 2 to whatever output pin is set on your attiny. Change baud rate or pins so it fits your setup. For a sample of how it looks on the attiny side, [see this repo](https://github.com/submarines-and/night-light/blob/master/src/main.cpp).