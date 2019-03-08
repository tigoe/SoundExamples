# MIDI Serial

The MIDI serial examples will work on any Arduino, with some modification. Boards that have two hardware serial ports, like the MKR boards, Mega, Leonardo, Micro, and Yún, can use Serial1 to send serial data to a MIDI playback device like a synthesizer or sampler. Serial1 is connected to a second asynchronous serial UART on these boards. (see the [setup page](setup.md) for more on asynchronous serial) Boards that don't have two hardware serial ports, like the Uno, can use the SoftwareSerial library to communicate with MIDI playback devices.

## Serial Output

The circuit you'll use for this approach is shown in Figure 1 below.The MIDI jack has 5 connectors arranged in a semi-circle. They are numbered, from left to right, 3,5,2,4,1 (don't ask why). The center pin (pin 3) of the MIDI connector is connected to ground. With the connector facing you the second pin from the left  (pin 5) is connected to the Arduno's serial transmit pin, labeled TX, through a 220-ohm resistor. On the MKR boards, this is the sixth pin from the top on the right side of the board. On a Leonardo or Yún, it's digital pin 1, the second from the bottom on the right. The MIDI jack's fourth pin from the left (pin 4) is connected to voltage through a 220-ohm resistor. The MIDI jack's center pin is connected to ground.

![Figure 1. MIDI serial output connection to a 5-pin MIDI connector.](img/midi_serial_out_bb.png)

*Figure 1. MIDI serial output connection to a 5-pin MIDI connector*

## For the Uno: MIDI SoftwareSerial

The Uno does not have a second UART serial port, so you'll need to use the SoftwareSerial library to send MIDI from it. Connect the MIDI jack as described in Figure 1, but connect the serial pin (**second pin from the left on the MIDI jack CHECK THIS**) to pin 3 on the Uno.


## Serial Input

Serial input is not as common a project, because the Arduino boards are better as controllers than as synthesizers. But you can read MIDI into an Arduino if you need to. Figure 2 shows the MIDI input circuit. The MIDI jack is connected to the microcontroller through an optoisolator. An *optoisolatator* has an LED on the input side and a phototransistor on the output side, so that the two sides of the circuit are electrically isolated from each other, for extra safety.

The parts for this are:
* 1 MIDI 5-pin connector
* 1 6N138 optoisolator
* 1 1N915 diode
* 1 270-ohm resistor
* 1 220-ohm resistor
* wires

In Figure 2, the optoisolator straddles the center of the breadboard a few rows below the MKR Zero board. It's an 8-pin component with pins on either side, spaced 0.1 inches apart (this is called a *DIP package*). The pins are numbered in a U-fashion with 1 on the top left, 4 on the bottom left, 5 on the bottom right, and 8 on the top right. The optoisolator's pin 8 (top right) is connected to voltage, and pin 5 (bottom right) is connected to ground. a 270-ohm resistor connects pin 6 to voltage as well. A wire connects the MKR Zero's RX pin (seventh from the top right) to the optoisolator's pin 6 as well. The cathode of a 1N914 diode is connected to the optoisolator's pin 3. The anode of the diode is connected to pin 2. A 220-ohm resistor connects pin 2 to the MIDI connector's fourth pin from the left (pin 4). The MIDI connector's second pin from the left (pin 5) is connected to the optoisolator's pin 3. Figure 2a shows the same connections in a schematic diagram. 
 
![Figure 2. MIDI serial input connection to a 5-pin MIDI connector through an optoisolator.](img/midi_serial_in_bb.png)

*Figure 2. MIDI serial input connection to a 5-pin MIDI connector through an optoisolator*

![Figure 2a. MIDI serial input schematic.](img/midi_serial_in_schem.png)

*Figure 2a. MIDI serial input schematic.*