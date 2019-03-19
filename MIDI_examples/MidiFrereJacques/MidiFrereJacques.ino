/*
  MIDI melody player

  Generates a series of tones from MIDI note values.
   The melody is "Frere Jacques"

  Circuit: none

  created 13 Feb 2017
  modified 23 Dec 2018
  by Tom Igoe
*/

#include <MIDIUSB.h>

#define NOTE_A4 69         // MIDI note value for middle A
#define FREQ_A4 440        // frequency for middle A

// the melody sequence:
int melody[] = {1, 3, 5, 1,
                1, 3, 5, 1,
                5, 6, 8, 5, 6, 8,
                8, 10, 8, 6, 5, 1,
                8, 10, 8, 6, 5, 1,
                1, -4, 1,
                1, -4, 1
               };
// the rhythm sequence:
int rhythm[] = {4, 4, 4, 4,
                4, 4, 4, 4,
                4, 4, 2,
                4, 4, 2,
                8, 8, 8, 8, 4, 4,
                8, 8, 8, 8, 4, 4,
                4, 4, 2,
                4, 4, 2
               };
// which note of the melody to play:
int noteCounter = 0;
int melodyLength = 32;

int bpm = 120;  // beats per minute
// duration of a beat in ms
float beatDuration = 60.0 / bpm * 1000;

int tonic = 65; // the tonic sets the key you're in

void setup() {

}

void loop() {
  // current note is an element of the array:
  int currentNote = melody[noteCounter] + tonic;
  // play a note from the melody:
  // convert MIDI note number to frequency:

  // calculate noteDuration from the note type:
  float noteDuration = beatDuration * (4.0 / rhythm[noteCounter]);
  // turn the note on:
  // play a note from the melody:
  midiCommand(0x90, currentNote, 127);
  // keep it on for the appropriate duration:
  delay(noteDuration * 0.85);
  // turn the note off:
  midiCommand(0x90, currentNote, 0);
delay(noteDuration * 0.15);
  // increment the note number for next time through the loop:
  noteCounter++;
  // keep the note in the range from 0 - melodyLength using modulo:
  noteCounter = noteCounter % melodyLength;
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
