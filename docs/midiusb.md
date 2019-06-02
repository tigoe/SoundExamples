# MIDI USB

These examples make your microcontroller show up to your personal computer as a MIDI controller. Thy will work on any of the USB-native Arduino boards, like the  MKR boards, Leonardo, Micro, Yún, and Nano 33 IoT. There's no special circuit for these examples. You just plug your board into your personal computer, upload a MIDI USB example, and it will appear to your computer as a MIDI controller.

In order to use these examples on a MKR Zero, MKR 1000, MKR 1010 or other board, you'll need to install the MIDIUSB library. In the Arduino IDE, click on the Sketch Menu, then choose Include Library... then Manage Libraries. This will open the Library Manager. Filter for MIDIUSB, and you should see a library called "MIDIUSB by Gary Grewal, Arduino". Install it. Then you're ready to start.

## Simple MIDI Player

Let's start with a simple MIDI melody player. We'll use the Steve Reich *Piano Phase* melody that you used in the [melody exercise](melody.md).  Start a new sketch by including the MIDIUSB library:

````
#include <MIDIUSB.h>
````

The song will be at 72bpm, and the melody will be the same as it was in the melody exercise, so your global variables will look like this:

````
int bpm = 72;  // beats per minute
// duration of a beat in ms
float beatDuration = 60.0 / bpm * 1000;

// the melody sequence:
int melody[] = {64, 66, 71, 73, 74, 66, 64, 73, 71, 66, 74, 73};
// which note of the melody to play:
int noteCounter = 0;
````

There's nothing you need to do in the `setup()` function, so keep it blank. 

````
void setup() {

}
````

In the `loop()` function, you'll send a MIDI command to play a note on channel 0 (that's MIDI command 0x90) at full volume (that's 127, or 0x7F in hexadecimal). You'll write the function later, but with that information, you can write the call to the function:

````
void loop() {
  // play a note from the melody:
  midiCommand(0x90, melody[noteCounter], 0x7F);
````
All the notes in this melody are sixteenth notes, which is 1/4 of a beat, so delay that long. Then send a note off command (0x80) for the same pitch:

````
 // all the notes in this are sixteenth notes,
  // which is 1/4 of a beat, so:
  int noteDuration = beatDuration / 4;
  // keep it on for the appropriate duration:
  delay(noteDuration);
  // turn the note off:
  midiCommand(0x80, melody[noteCounter], 0);
  ````

  Finally, add one to the note counter, then make sure it's no larger than the length of the melody (12 notes). That'll be the end of the `loop()` function:

  ````
    // increment the note number for next time through the loop:
  noteCounter++;
  // keep the note in the range from 0 - 11 using modulo:
  noteCounter = noteCounter % 12;
}
````

Now you need to write the `midiCommand()` function that you've called in the loop. It looks like this:

````
// send a 3-byte midi message
void midiCommand(byte cmd, byte data1, byte  data2) {
  // First parameter is the event type (top 4 bits of the command byte).
  // Second parameter is command byte combined with the channel.
  // Third parameter is the first data byte
  // Fourth parameter second data byte

  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
````

When you upload this, it will send out MIDI notes over and over. 

### What's With The >> ?
That strange operator, `>>` is called the [bit shift-right](https://www.arduino.cc/reference/en/language/structure/bitwise-operators/bitshiftright/) operator. It shifts all the bits in the number or variable you're shifting to the right. Since each bit position is a power of 2 , bit shifting to the right is the same as dividing by 2. So `cmd >> 4` shifts the number by 2<sup>4</sup>, or 16. Remember how 16 is a special number in MIDI? So if the MIDI command value you sent was 0x90, then when it's shifted by 16, you have 0x9, which is the command value independent of which channel you're playing. The MIDIUSB library needs that number, so that's why you shifted it in the function.

## Connecting The Controller to a MIDI Synth

Even though your device is sending out MIDI, no one's listening. If you have a Digital Audio Workstation like Ableton or GarageBand, then you can open it, configure the MIDI input to look for your Arduino, and the notes will play. For more on that, see this [MIDI to DAW tutorial](https://itp.nyu.edu/physcomp/labs/labs-serial-communication/lab-arduino-to-daw/). If you're not using a DAW, download a free MIDI synth player like [Sforzando](https://www.plogue.com/products/sforzando.html). You might want the [Sforzando users guide](https://s3.amazonaws.com/sforzando/sforzando_guide.pdf) as well. Then download an instrument file. This [Yamaha Disklavier grand piano](http://freepats.zenvoid.org/Piano/acoustic-grand-piano.html) is pretty good, as are some of the other SoundFonts on that page. 

Launch Sforzando. The first time you launch it, it should prompt you to set your MIDI input preferences. Choose your Arduino as a MIDI input. Unzip the sound files, then click Import in Sforzando to open the SoundFont files.  Then you should hear the melody playing. Congratulations, you've got MIDI! Here is [the complete sketch](https://github.com/tigoe/SoundExamples/blob/master/MIDI_examples/MIDIUSB_simple/MIDIUSB_simple.ino).

Now you're ready to go on to [making a MIDI instrument](midi-instrument.md).

## Things to Remember

If you're using the MIDIUSB library, there are just a few things you'll want to remember as you build other examples:

### Include the MIDIUSB library

This should be at the top of every MIDIUSB sketch:

````
#include <MIDIUSB.h>
````

### Make Notes with a MIDI Command Function

Most MIDI commands are three bytes, a command byte followed by two data bytes. So the `MidiCommand()` function you wrote here should work for most MIDI note-playing applications using MIDIUSB:


````
// send a 3-byte midi message
void midiCommand(byte cmd, byte data1, byte  data2) {
  // First parameter is the event type (top 4 bits of the command byte).
  // Second parameter is command byte combined with the channel.
  // Third parameter is the first data byte
  // Fourth parameter second data byte

  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
````

It's worth noting that the MIDIUSB library also includes a file that gives you note names for the MIDI values. So if you want to write, for example, `pitchA4` instead of `69`, then include this file at the top of your sketch after the MIDIUSB library like so:

````
#include <pitchToNote.h>
````