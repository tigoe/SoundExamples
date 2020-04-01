/*
  Controller explorer

  This sketch lets you start a note, let it play, and then explore different controllers'
  effect on the note. Pressing the note button starts or stops the note. Pressing the
  controller button increments the controller number that's being applied to the note.
  Turning the potentiometer changes the current controller value.

  It's ideal for exploring the various effects of controllers on different soft synths

  created 29 Mar 2020
  by Tom Igoe
*/

#include <MIDIUSB.h>      // Include MIDIUSB library
#include <PushButton.h>   // include PushButton library https://github.com/tigoe/PushButton
#include <pitchToNote.h>  // include pitch chart from MIDIUSB library

int droneState = 0;       // whether you are droning or not
int controllerNumber = 0; // current controller number
int note = pitchC4;       // pitch being played, from the MIDIUSB pitch chard

PushButton noteButton(2);    // note button
PushButton controllerButton(3);  // controller Button

void setup() {
  Serial.begin(9600);
  // start the buttons as INPUT_PULLUP (the default for PushButton library)
  noteButton.begin();
  controllerButton.begin();
}

void loop() {
  // read joystick
  int sensor = analogRead(A0);

  // if the note button is pressed:
  if (noteButton.pressed()) {
    // if you're not currently droning, start.
    // if you're currently droning, stop.
    if (!droneState) {
      // turn on droning note
      midiCommand(0x90, note, 0x45);
      droneState = true;
    } else {
      // turn off droning note
      midiCommand(0x80, note, 0x00);
      droneState = false;
    }
  }

  // if the controller button is pressed,
  // increment the controller number:
  if (controllerButton.pressed()) {
    controllerNumber++;
    // limit the controller number to 0 - 127:
    controllerNumber %= 128;
    Serial.println(controllerNumber);
  }
  // if the drone state is on, read the potentiometer
  // and use its value as the controller number:
  if (droneState) {
    int controllerValue = sensor / 8;
    midiCommand(0xB0, controllerNumber, controllerValue);
  }
}
void midiCommand(byte cmd, byte data1, byte  data2) {
  // First parameter is the event type (top 4 bits of the command byte).
  // Second parameter is command byte combined with the channel.
  // Third parameter is the first data byte
  // Fourth parameter second data byte
  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
}
