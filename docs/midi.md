#  Introduction to MIDI, the Musical Instrument Digital Interface Protocol

*Note: There are two different types of MIDI examples in this repository: MIDI serial examples and MIDI USB examples.* 

*The MIDI serial examples will work on any Arduino, with some modification. Those boards that have two hardware serial ports, like the MKR boards, Mega, Leonardo, Micro, and Yún, can use Serial1 to send serial data to a MIDI playback device like a synthesizer or sampler. THose boards that don't have two hardware serial ports, like the Uno, can use the SoftwareSerial library to communicate with MIDI playback devices.*

*The MIDI USB examples will work on any of the USB-native Arduino boards, like the  MKR boards, Leonardo, Micro, and Yún. These examples use the MIDIUSB library to turn these boards into a MIDI USB controller. This is useful when you want to communicate with software on a personal computer. Digital Audio Workstation (DAW) software, such as GarageBand, ProTools, Logic, or other MIDI software like Max/MSP, can recognize MIDI USB controllers and play sound based on their input.*