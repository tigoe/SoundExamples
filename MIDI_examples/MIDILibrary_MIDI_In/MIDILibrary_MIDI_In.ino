/*
  MIDi Input using FortySevenEffects' MIDI library.
  For details of the library see
  https://github.com/FortySevenEffects/arduino_midi_library

  Uses callback functions to handle incoming MIDI messages on Serial1
  Works on all boards with 2 hardware serial ports (MKR, Leonardo, Micro, Due, etc)

  created 8 Mar 2019
  by Tom Igoe

  Based on Callback example from FortySevenEffects

*/
#include <MIDI.h>

// create a MIDI input instance using Serial1:
MIDI_CREATE_DEFAULT_INSTANCE();


void setup() {
  // set up callback functions for noteOn,noteOff, and pitchBend:
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandlePitchBend(handlePitchBend);

  // Initiate MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(9600);
}

void loop() {
  MIDI.read();
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  Serial.print("noteon channel 0x");
  Serial.print(channel, HEX);
  Serial.print(" pitch 0x");
  Serial.print(pitch, HEX);
  Serial.print(" velocity 0x");
  Serial.println(velocity, HEX);
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  Serial.print("noteoff channel 0x");
  Serial.print(channel, HEX);
  Serial.print(" pitch 0x");
  Serial.print(pitch, HEX);
  Serial.print(" velocity 0x");
  Serial.println(velocity, HEX);
}

void handlePitchBend(byte channel, int bend) {
  Serial.println("pitch bend channel 0x");
  Serial.print(channel, HEX);
  Serial.print(" bend 0x");
  Serial.println(bend, HEX);
}
