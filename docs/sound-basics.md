
# The Basics of Making Sounds

*Note: The [Tone examples](https://github.com/tigoe/SoundExamples/tree/master/Tone_examples) mentioned here will play simple tones on any Arduino board. You can't play multiple tones at the same time, however. These examples are included mainly to show how to convert between frequency values, note names, and MIDI note values.*

In order to use these examples, there are a few things you need to know. 

Sound is created by vibrations in air. When those vibrations get fast enough, above about 20 times a second, you hear them as a pitch. The number of vibrations per second is called the **frequency** and frequency is measured in Hertz (Hz). So 20 times a second is 20Hz. Humans can hear pitches from about 20Hz to about 20,000Hz, or 20 kiloHertz (kHz).

What's vibrating? A speaker can vibrate. The paper cone of the speaker moves forward, then backward, than back to its resting place. The length of time it takes to move from its resting place to one extreme, then to the other, then to its resting place again, is called the **wavelength** of the vibration. Sometimes it's also called the **period**. For example, if a speaker is vibrating at 20Hz, then it moves forward, backward, and back to rest in 1/20 of a second, or 0.05 seconds. 0.05 seconds is the period, or the wavelength, of the sound you're hearing.

A computer makes sound by sending pulses of electrical energy through a wire that's connected to the paper cone of a speaker. That wire is wrapped in a coil, and mounted inside a magnet. The electrical energy generates a magnetic field, and that field is either attracted to the magnet or repelled by it, depending on which direction the electrical energy is flowing. The magnetic energy moves the coil, and since the coil is attached to the cone, the speaker moves.

Arduino modules are simple computers. They have input and output pins, and you can use one of those pins to vibrate a speaker.  To get started doing that, connect a speaker to your Arduino MKR Zero (or any of the MKR boards) as shown below

## Connecting A Speaker or Headphone Jack to the Arduino

Attach one end of a 220-ohm resistor to pin 5 of the MKR Zero. Attach the other end of the resistor to one of the empty rows on your breadboard. Next attach one wire of the speaker in that same row. Then attach the other wire of the speaker to the MKR Zero's ground (GND) pin. Figure 1 below shows what that would look like on a [solderless breadboard](https://itp.nyu.edu/physcomp/labs/labs-electronics/breadboard/)

![Figure 1. Speaker attached to pin 5 of a MKR Zero](img/speaker-circuit_bb.png)

*Figure 1. Speaker attached to pin 5 of a MKR Zero. The resistor limits the current to the speaker to protect it.*

You can also use headphones if you like. To do this, connect one end of a 10-kilohm resistor to pin 5 of the MKR Zero. Then connect the other end to one of the empty rows on your breadboard. Next attach the left or right inputs to your audio jack (usually the outside pins) in that same row. Then attach the audio jack's ground pin (usually the middle pin) to the MKR Zero's ground (GND) pin. Figure 2 below shows what that would look like on a solderless breadboard:

![Figure 2. headphone jack attached to pin 5 of a MKR Zero](img/headphone-circuit_bb.png)

*Figure 2. Headphone jack attached to pin 5 of a MKR Zero. The resistor limits the current and lowers the voltage to the headphone jack to protect your headphones and your ears.*
