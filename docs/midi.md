#  MIDI, an Overview

Making sound from a microcontroller has its limits. Without more complex programming, you can't make polyphonic sounds. Building a synthesizer from scratch is a significant challenge. Controlling a synthesizer from a microcontroller, on the other hand, is not difficult. The **Musical Instrument Digital Interface protocol (MIDI)**, makes it a straightforward task. MIDI is the way musical computers communicate with each other.

MIDI developed in the 1980s, as music equipment started to go digital. The sounds of 80's New Wave is the sound of MIDI synthesizers. The protocol was designed to make the keyboards and [keytars](https://en.wikipedia.org/wiki/Keytar) and other instruments compatible with synthesizers and samplers and other devices. 

MIDI doesn't carry actual sound data, like an MP3 file or a WAV file. MIDI *describes* music, kind of like sheet music does. A MIDI controller is a device that can generate MIDI messages, and a MIDI playback device like a synthesizer or sampler receives those messages and generates sounds in response to them. In the exercises that follow, you'll build MIDI controllers that communicate with existing MIDI synthesizers.

Synthesizers used to come exclusively encased in keyboards or rack-mounted boxes. While you can still buy those, it's just as common to use software synthesizers that can run on your personal computer, tablet, or phone. You can also get MIDI synthesizer modules that attach to a breadboard or to an Arduino directly. The VS1053 from VSLI is one such component. It's a processor about the size of the MKR Zero's central processor that can act as a general MIDI synthesizer or MP3 playback device. You'll put it into action later on.

## A Brief Intro to MIDI Commands

There are many parts to the MIDI protocol, but here are some basics to get you started.

A MIDI message is a series of bytes sent from a controller to a playback device using [asyncnronous serial communication](setup.md#serial-communication). Each byte of a MIDI message is either a **command byte** or  **status byte**. Every MIDI message starts with a command byte that's like the verb of a sentence (or a programming function), followed by followed by status bytes, which are the modifiers of the command (or the function parameters). MIDI command bytes always have values are between 128 and 255, and status bytes always have values  between 0 and 127. 

<u style="color: green;">Syntax note:</u>  MIDI message bytes are usually written in **hexadecimal notation** Hexadecimal means base 16, which means you have 16 digits: 0 through 9, then A through F (A = 10 in base 10, B = 11, C = 11, and so forth until F = 15). Each place is a power of 16, just as each place in base 10 is a power of ten. In hexadecimal notation, the number 127 is written 0x7F (16 * 7 + 15). 128 is 0x80, or 8 * 16. 

MIDI sounds are organized in **channels**, with each channel being a different instrument. Channels can be organized into **banks** of instrument styles. MIDI is often organized in groups of 16: groups of instruments are generally groups of no more than 16 (for example, the [General MIDI Sound Set](https://www.midi.org/specifications-old/item/gm-level-1-sound-set)). This makes hexadecimal notation handy. 

For example, to play a note on a piano, which is usually the first channel (or channel 0) you send a note-on command byte followed by a pitch status byte and a velocity, or volume, status byte. So note on, middle A, medium loud would be:

* 0x90 (note on, channel 0)
* 0x45 (69 in base 10, or middle A, 440Hz)
* 0x40 (64 in base 10, or medium loud)

Note off for the same note on the same channel would be:

* 0x80 (note off, channel 0)
* 0x45 (69 in base 10, or middle A, 440Hz)
* 0x00 (0 no sound)

You could also send a second note on message, at velocity 0.

To change the instrument, you change the channel in the note on message. The channel is the lower digit of the message, so note on, channel 1, middle A would be:

````
0x91 0x45 0x40
````

You'll see some other MIDI messages in the exercises that follow, but the noteon and noteoff messages are the best place to start. 

## MIDI Software

There are lots of options for MIDI input personal computers, from freeware **software synths** like [SimpleSynth](http://notahat.com/simplesynth/) (MacOS) for MacOS, or the cross-platform [Sforzando](https://www.plogue.com/downloads.html#sforzando) for Windows, MacOS and Linux. There are also fully-featured multi-track **digital audio workstation** applications that can take MIDI data and play software synths and record analog or digital audio input at the same time, and output multi-track recordings. Pro Tools, Ableton Live and GarageBand for MacOS, are examples of DAW software. There are  also programming environments that can take MIDI input via USB or network. Max/MSP and Pure Data (Max' open source cousin) are examples of these. You can get started at any level you want, but if you're new to MIDI, than the simplest and least expensive way is probably to download a free software synth.

Synthesizers, both the sofware kind and the hardware kind, take MIDI input and synthesize different instrument sounds using the data. Software synths are just applications running on a personal computer, and can be expanded to play new sounds using a file format called [SoundFonts](http://www.sfzformat.com/). These expand what a software synth app can do similarly to how text fonts expand what a word processor can do. A soundFont might give your software synth app a new piano or guitar sound, for example.

## Approaches to MIDI Controllers

There are three approaches to building MIDI controllers using Arduino covered here, depending on what kind of playback device you plan to use. 

### MIDI USB 

If you're communicating with a software synthesizer or digital audio workstation, you'll want the [**MIDI USB examples**](midiusb.md). These examples make your microcontroller appear to your personal computer as a MIDI controller. The MIDI USB examples will work on any of the USB-native Arduino boards, like the  MKR boards, Leonardo, Micro, and Yún. They'll work on the Nano 33 IoT as well. There's no special circuit for these examples. You just plug your board into your personal computer, upload a MIDI USB example, and it will appear to your computer as a MIDI controller.

### MIDI Bluetooth

The [MIDI Bluetooth examples](midi-ble.md) will work on a Bluetooth-enabled Arduino like the MKR 1010 or the Nano 33 IoT. They communicate with your computer using Bluetooth LE. They've only been tested on MacOS.

### MIDI Serial

If you're communicating with a piece of MIDI equipment like a synthesizer or a sampler, you'll need the [**MIDI serial examples**](midi-serial.md). These examples use the asynchronous Serial commands to send MIDI data over the TX pin of the board to a synth or sampler. You'll also need a 5-pin MIDI jack for them. 

The MIDI serial examples will work on any Arduino, with some modification. 

### MIDI Serial to a VS1053 Synth Module

 [VSLI's VS1053 MP3 decoder and MIDI synthesizer](http://www.vlsi.fi/en/products/vs1053.html) is a synthesizer module that can be mounted on a breadboard, or connected directly to a microcontroller. It can also play MP3 sound files. Sparkfun and Adafruit both make breakout boards or Arduino shields for this component. The [MIDI VS1053 page](midi-vs1053.md) covers this approach. 

Browse the reference links below, then try either the [MIDI USB](midiusb.md), [MIDI Bluetooth](midi-ble.md), [MIDI Serial](midi-serial.md), or [MIDI using VS1053](midi-vs1053.md) examples to try it out. 

## Converting from one  MIDI Approach to Another

All of the MIDI examples in this repository are written so that it should be relatively easy to convert from one MIDI form to another. The MIDI command functions are separated from the main loop function that you can swap one approach for another with little code change. 

## MIDI References

The full MIDI protocol is detailed on the [MIDI Manufacturers' Association page](https://www.midi.org).

The [General MIDI specification](https://www.midi.org/specifications-old/item/general-midi) lists all a general synth should be able to do. 
The [General MIDI instrument specification](https://www.midi.org/specifications-old/item/gm-level-1-sound-set) lists all the instruments that should be available on a general MIDI synth. 

[MIDI Pitch Bending ](midi-pitch-bend.md) because why stay with just well-tempered notes?

[MIDI Bit Shifting](midi-bit-shifting.md) because you often need to convert 8-bit values to 7-bit bytes in MIDI

## MIDI Exercises

* [MIDI USB Intro](midiusb.md)
* [Making A MIDI Instrument](midi-instrument.md)

## MIDI Software Links

SoundFont players:
* [Sforzando](https://www.plogue.com/downloads.html#sforzando) (MacOS and Windows)
* [SimpleSynth](http://notahat.com/simplesynth/) (MacOS)
* [SynthFont](http://www.synthfont.com/Downloads.html)

Free soundFont files:

* [FreePats](http://freepats.zenvoid.org/index.html)
* [Aria Engine](http://ariaengine.com/free-sfz-sounds/)
* [PatchArena](http://patcharena.com/tag/free-sfz-instruments/)

Digital Audio Workstations (not free):
* A good [comparison of the major DAWs](https://ehomerecordingstudio.com/best-daw-software/)
* [Ableton Live](https://www.ableton.com/en/live/compare-editions/)
* [ProTools](https://www.avid.com/pro-tools)
* [Adobe Audition](https://www.adobe.com/products/audition.html)
* [Logic Pro X](https://www.apple.com/logic-pro/) (MacOS)

MIDI Programming IDEs:
* [Pure Data](http://puredata.info/downloads/pure-data) (free)
* [Pd manual](http://write.flossmanuals.net/pure-data/introduction2/)
* [Max/MSP](https://cycling74.com/downloads) (not free)


