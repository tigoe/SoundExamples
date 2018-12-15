# Sound Examples for Arduino

A collection of sound examples for Arduino. There are MIDI examples, tone examples, and examples for sound playback components.

These examples were written using the MKR Zero and other MKR boards, though many of the of them will work on other boards as well. Each example notes any restrictions on which boards are required. 

The WAV file playback examples for the MKR Zero and other MKR boards use the ArduinoSound library. This library uses the I2S protocol to communicate with different I2S-compatible amplifiers and microphones.

The examples using [VSLI's VS1053 MP3 decoder](http://www.vlsi.fi/en/products/vs1053.html) and MIDI sound module use the Adafruit VS1053 library. This library will work with any of the modules from Sparkfun or Adafruit or others that use the VS1053 module. 

There are two different types of MIDI examples in this repository: MIDI serial examples and MIDI USB examples. 

The MIDI serial examples will work on any Arduino, with some modification. Those boards that have two hardware serial ports, like the MKR boards, Mega, Leonardo, Micro, and Yún, can use Serial1 to send serial data to a MIDI playback device like a synthesizer or sampler. THose boards that don't have two hardware serial ports, like the Uno, can use the SoftwareSerial library to communicate with MIDI playback devices.

The MIDI USB examples will work on any of the USB-native Arduino boards, like the  MKR boards, Leonardo, Micro, and Yún. These examples use the MIDIUSB library to turn these boards into a MIDI USB controller. This is useful when you want to communicate with software on a personal computer. Digital Audio Workstation (DAW) software, such as GarageBand, ProTools, Logic, or other MIDI software like Max/MSP, can recognize MIDI USB controllers and play sound based on their input. 

The Tone examples will play simple tones on any Arduino board. You can't play multiple tones at the same time, however. These examples are included mainly to show how to convert between frequency values, note names, and MIDI note values.
