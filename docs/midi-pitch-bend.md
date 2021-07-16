# MIDI Pitch Bend

MIDI normally only allows the notes of a traditional western scale. The MIDIUSB library includes [a table of MIDI note numbers to note names](https://github.com/arduino-libraries/MIDIUSB/blob/master/src/pitchToNote.h), telling you what notes you can play with noteOn and noteOff messages. But you can get other pitches using pitch bend. 

You play a note on channel 0 with a command like this:

````
midiCommand(0x90, pitch, velocity);
````

Then to bend the pitch on channel 0, you send a pitch bend message like this:

````
midiCommand(0xE0, lsb, msb);
````

`lsb` and `msb` are the least significant byte and most significant byte of a 14-bit number. Note that most computers store numbers as 8-bit numbers, so you'll need to [convert the bytes](midi-bit-shifting.md) before you send them.  

Pitch bend can bend up or down. Most synthesizers will bend up to +/-2 semitones. So max pitch bend down takes the note down two semitones. Max pitch bend up takes it up two semitones. A pitch bend value of 8192 represents no pitch bend. For example, if you want a pitch between C4 and C#4, you would play C4, then pitch bend up a little. For example:

````
midiCommand(0x90, 60, 127);  // play C4 as loud as possible
midiCommand(0xE0, 39, 64);  // a little pitch bend up
````
Or to play between C4 and B3, the note below it, you would pitch bend down:

````
midiCommand(0x90, 60, 127);  // play C4 as loud as possible
midiCommand(0xE0, 100, 63);  // a little pitch bend down
````
How do you get the pitch bend values? Pitch bend has a 14-bit range, from 0 - 16383. The two data bytes combined make the pitch bend value. A pitch bend of 0 bends 2 semitones down, while 16383 bends almost 2 semitones up. In other words, C4 plus pitch bend of 16383 is D4, and C4 plus pitch bend of 0 is Bb3.  

Now, you might think, "okay, I'll just read an analog sensor and map it from 0-1023 to 0-16384". That's a start, but remember, MIDI data bytes are 7-bit, not 8-bit, like normal. This is where you need to know about converting 8-bit values to 7-bit. This is where the conversion of 8-bit values to 7-bit for MIDI (midi-bit-shifting.html) will help. 

For a simple pitch bend example, see the [MIDIUSBPitchBend example](https://github.com/tigoe/SoundExamples/blob/master/MIDI_examples/MIDIUSBPitchBend/MIDIUSBPitchBend.ino). 
