/*
  One-key piano MIDI player for MIDUSB

  Generates a  MIDI notes when you push the button.

  Uses MIDIUSB for MIDI, so will work on any
  32U4-based board (e.g. Uno, Leonardo, Micro, Yún)

  Circuit:
      pushbutton attached to +Vcc from pin 5.
      10-kilohm resistor to ground from pin 5

  created 14 Jan 2019
  by Tom Igoe
*/
#include <MIDIUSB.h>

int lastButtonState = HIGH;

void setup() {
  pinMode(5, INPUT);
}

void loop() {
  int buttonState = digitalRead(5);
  if (buttonState != lastButtonState) {
    delay(5);
    if (buttonState == HIGH) {
      midiCommand(0x90, 0x45, 0x7F);
    } else {
      // turn the note off:
      midiCommand(0x80, 0x45, 0);
    }
    // save the current state as the previous state
    // for the next comparison:
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
