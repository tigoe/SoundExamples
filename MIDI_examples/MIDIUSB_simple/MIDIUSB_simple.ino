/*
  Simple MIDI player for MIDUSB

  Generates a series of MIDI notes.
   The melody is Steve Reich's "Piano phase"

  Uses MIDIUSB for MIDI, so will work on any
  32U4-based board (e.g. Uno, Leonardo, Micro, Yún)

  Circuit:
    none
    
  created 13 Feb 2017
  modified 13 Dec 2018
  by Tom Igoe
*/
#include <MIDIUSB.h>

int bpm = 72;  // beats per minute
// duration of a beat in ms
float beatDuration = 60.0 / bpm * 1000;

// the melody sequence:
int melody[] = {64, 66, 71, 73, 74, 66, 64, 73, 71, 66, 74, 73};
// which note of the melody to play:
int thisNote = 0;

void setup() {

}

void loop() {
  // play a note from the melody:
  midiCommand(0x90, melody[thisNote], 127);
  // all the notes in this are sixteenth notes,
  // which is 1/4 of a beat, so:
  int noteDuration = beatDuration / 4;
  // keep it on for the appropriate duration:
  delay(noteDuration);
  // turn the note off:
  midiCommand(0x80, melody[thisNote], 127);
  // increment the note number for next time through the loop:
  thisNote++;
  // keep the note in the range from 0 - 12 using modulo:
  thisNote = thisNote % 12;
}

// send a 3-byte midi message
void midiCommand(byte cmd, byte data1, byte  data2) {
  // First parameter is the event type (top 4 bits of the command byte).
  // Second parameter is command byte combined with the channel.
  // Third parameter is the first data byte
  // Fourth parameter second data byte

  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
}
