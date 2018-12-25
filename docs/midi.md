#  Introduction to MIDI, the Musical Instrument Digital Interface Protocol

Making sound from a microcontroller has its limits. Without more complex programming, you can't make more polyphonic sounds. Building a synthesizer from scratch is a significant challenge. Controlling a synthesizer from a microcontroller, on the other hand, is not difficult. The **Musical Instrument Digital Interface protocol (MIDI)**, makes it a straightforward task. MIDI is the way musical computers communicate with each other.

MIDI developed in the 1980s, as music equipment started to go digital. The sounds of 80's New Wave is the sound of MIDI synthesizers. The protocol was designed to make the keyboards and keetars and other instruments compatible with synthesizers and samplers and other devices. A MIDI message isn't actually sound data, like an MP3 file or a WAV file. MIDI *describes* music, kind of like sheet music does. A MIDI controller is a device that can generate MIDI messages, and a MIDI playback device like a synthesizer or sampler receives those messages and generates sounds in response to them. In the exercises that follow, you'll build MIDI controllers that communicate with existing MIDI synths.

Synthesizers used to come encased in keyboards or rack-mounted boxes. While you can still buy those, it's just as common to use software synthesizers that can run on your personal computer, tablet, or phone. You can also get MIDI synthesizer modules that attach to a breadboard or to an Arduino directly. The VS1053 from VSLI is one such component. It's a processor about the size of the MKR Zero's central processor that can act as a general MIDI synthesizer or MP3 playback device. You'll put it into action later on. 

## Three Approaches to MIDI Controllers

There are three approaches to building MIDI controllers, depending on what kind of playback device you plan to use. 

## MIDI Software

There are lots of options for MIDI input personal computers, from freeware **software synths** like SimpleSynth for MacOS, or the cross-platform [Sforzando](https://www.plogue.com/downloads.html#sforzando) for Windows, MacOS and Linux. There are also fully-featured multi-track **digital audio workstation** applications that can take MIDI data and play software synths and record analog or digital audio input at the same time, and output multi-track recordings. Pro Tools, Ableton Live andGarageBand for MacOS, are examples of DAW software. There are  also programming IDEs that can take MIDI input via USB or network. Max/MSP (now owned by Ableton) and Pure Data (Max' open source cousin) are examples of these. You can get started at any level you want, but if you're new to MIDI, than the simplest and least expensive way is probably to download a free software synth like Sforzando.

 Most software synths can be expanded to play new sounds using a file format called soundFonts. [SoundFonts](http://www.sfzformat.com/) expand what a synthesizer can do similarly to how text fonts expand what a word processor can do. 


*Note: There are two different types of MIDI examples in this repository: MIDI serial examples and MIDI USB examples.* 

*The MIDI serial examples will work on any Arduino, with some modification. Those boards that have two hardware serial ports, like the MKR boards, Mega, Leonardo, Micro, and Yún, can use Serial1 to send serial data to a MIDI playback device like a synthesizer or sampler. THose boards that don't have two hardware serial ports, like the Uno, can use the SoftwareSerial library to communicate with MIDI playback devices.*

*The MIDI USB examples will work on any of the USB-native Arduino boards, like the  MKR boards, Leonardo, Micro, and Yún. These examples use the MIDIUSB library to turn these boards into a MIDI USB controller. This is useful when you want to communicate with software on a personal computer. Digital Audio Workstation (DAW) software, such as GarageBand, ProTools, Logic, or other MIDI software like Max/MSP, can recognize MIDI USB controllers and play sound based on their input.*