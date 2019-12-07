# Overview:
This folder contains the code and design for the hardware token used in the ECE 
180DA senior capstone project. It provides users of the system an easy way to
communicate their physical locations with the server. Three seven segment displays
are used to communicate whether the row or column is currently being modified as
well as the position for that entry. A toggle switch is used to swap between rows
and columns and two push buttons are used to increment and decrement the display.
There is an LED indicator that upon receipt of success from the server will tell
the user that the row/col transmission was successful and that the token can be
passed to the next client. If you wish to design a token for yourself consult the
following parts list.

# Project Materials:
The following materials are integral to creating and using the hardware token.
Values on resistors and capacitors can be reasonably flexible. 220 ohm resistors
work well instead of the 1k ohm for the LEDs and 7 segment displays. The 10k ohm,
1k ohm and 1 uF capacitors are used for button debouncing and can be modified if
need be. The time constant obtained from the configuration I have works well, but
it isn't the only design that works.
* 1 x Raspberry Pi Zero W
* 3 x Common Cathode 7 segment displays
* 2 x 1uF capacitors
* 2 x Push Buttons
* 1 x LED
* 1 x Toggle Switch
* 5 x 1k Ohm Resistors
* 3 x 10k Ohm Resistors
* 1 x ProtoBoard (Breadboard works too, but components should ideally be soldered)

# Wiring:
![Circuit Schematic](Schematic/circuit.png?raw=true "Wiring Diagram")
Note: The above circuit schematic is under the assumption you are not modifying the 
GPIO pins in the files included. If you wish to change the pin ordering while wiring
up the circuit simply place the corresponding wiringPI GPIO pin number in the GPIO
pins located in hardware.h. For a diagram mapping GPIO pins to WiringPI GPIO click 
[here](https://pinout.xyz/#).

# NOTICE:
This is currently under development. The following have yet to be implemented:
* 3D printed case for housing all of the components.
* PCB to facilitate soldering