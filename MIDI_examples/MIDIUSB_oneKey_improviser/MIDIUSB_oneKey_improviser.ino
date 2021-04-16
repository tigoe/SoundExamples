/*
  One-key piano MIDI player for MIDUSB

  Generates a  MIDI notes when you push the button.
  Randomizes the note within a scale each time

  Uses MIDIUSB for MIDI, so will work on any
  32U4- or SAMD-based board (e.g. Nano 33 IoT, MKR Leonardo, Micro, Yún)

  Circuit:
      pushbutton attached to +Vcc from pin 5.
      10-kilohm resistor to ground from pin 5

  created 14 Jan 2019
  by Tom Igoe
*/
#include <MIDIUSB.h>
#include <pitchToNote.h>
const int buttonPin = 5;

// the intervals in a major and natural minor scale:
int major[] = {2, 2, 1, 2, 2, 2, 1};
int naturalMinor[] = {2, 1, 2, 2, 1, 2, 2};
// an array to hold the final notes of the scale:
int scale[8];

// start with middle C:
int tonic = pitchC3;
// note to play:
int noteValue = tonic;

// previous state of the button:
int lastButtonState = LOW;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  // fill the scale array with the scale you want:
  // start with the initial note:
  scale[0] = tonic;
  int note = scale[0];
  // iterate over the intervals, adding each to the next note
  // in the scale. You can change major to naturalMinor
  // if you want that kind of scale instead:
  for (int n = 0; n < 7; n++) {
    note = note + major[n];
    scale[n + 1] = note;
  }
}

void loop() {
  // read the pushbutton:
  int buttonState = digitalRead(buttonPin);
  // compare its state to the previous state:
  if (buttonState != lastButtonState) {
    // debounce delay:
    delay(5);
    // if the button's changed and it's pressed:
    if (buttonState == LOW) {
      // pick a random note in the scale:
      noteValue = scale[random(8)];
      // play it:
      midiCommand(0x90, noteValue, 0x7F);
    } else  {
      // turn the note off:
      midiCommand(0x80, noteValue, 0);
    }
    // save the button state for comparison next time through:
    lastButtonState = buttonState;
  }
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
