/*
  4-key MIDI piano controller

  Reads eight pushbuttons, and when any of them have changed, sends
  MIDI noteon or noteoff messages. Sends an all notes off message
  when all buttons are released.


  Uses MIDIUSB for MIDI, so will work on any
  32U4-based board (e.g. Uno, Leonardo, Micro, Yún)

  Circuit:
      pushbutton attached to +Vcc from pins 0 through 7
      10-kilohm resistor to ground from pins 0 through 7
      
  created 13 Feb 2017
  modified 14 Jan 2019
  by Tom Igoe
*/
#include <MIDIUSB.h>
#include <pitchToNote.h>

int keys[] = {0, 1, 2, 3, 4, 5, 6, 7}; // four pins for pushbuttons (keys)
int lastKeyState[] = {0, 0, 0, 0, 0, 0, 0, 0}; // previous state of the keys
int keyCount = 8;             // number of keys
int baseNote = pitchC4;      // C4, used as the base note
int noteValue = baseNote;

void setup() {
  for (int k = 0; k < keyCount; k++) { // iterate over the keys
    pinMode(keys[k], INPUT);           // make each an input
  }
}

void loop() {
  for (int k = 0; k < keyCount; k++) { // iterate over the keys
    // read the pushbutton:
    int keyState = digitalRead(keys[k]);
    // compare its state to the previous state:
    if (keyState != lastKeyState[k]) {
      // debounce delay:
      delay(5);
      // if the button's changed and it's pressed:
      if (keyState == HIGH) {
        // pick the note:
        noteValue = baseNote + k;
        // play it:
        midiCommand(0x90, noteValue, 0x7F);
      } else  {
        // turn the note off:
        midiCommand(0x80, noteValue, 0);
      }
      // save the button state for comparison next time through:
      lastKeyState[k] = keyState;
    }
  }
}

// send a 3-byte midi message
void midiCommand(byte cmd, byte data1, byte  data2) {
  Serial1.write(cmd);     // command byte (should be > 127)
  Serial1.write(data1);   // data byte 1 (should be < 128)
  Serial1.write(data2);   // data byte 2 (should be < 128)
}
