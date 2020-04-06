/*
  MIDI melody player

  Generates a series of tones from MIDI note values.
   The melody is "Frere Jacques". A potentiometer attached 
   to pin A0 allows you to take the piano out of tune.

  Circuit: potentiometer attached to A0

  created 3 Apr 2020
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
  // detune the piano:
  detune();
  // keep it on for the appropriate duration:
  delay(noteDuration * 0.85);
  // turn the note off:
  midiCommand(0x80, currentNote, 0);
delay(noteDuration * 0.15);
  // increment the note number for next time through the loop:
  noteCounter++;
  // keep the note in the range from 0 - melodyLength using modulo:
  noteCounter = noteCounter % melodyLength;
}

void detune() {
   int sensor = analogRead(A0);
  // pitch bend range is 0 - 16383, and
  // the middle of the range, 8192, is zero pitch bend:
  int pitchBend = map(sensor, 0, 1023, 0, 16383);
  
  // convert 8-bit bytes to 7-bit bytes:
  pitchBend = pitchBend << 1;          // shift low byte's msb to high byte
  byte msb = highByte(pitchBend);      // get the high bits
  byte lsb = lowByte(pitchBend) >> 1;  // get the low 8 bits

  // send the pitch bend message:
  midiCommand(0xE0, lsb, msb);
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
