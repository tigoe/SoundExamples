/*
  One-key MIDI player for MIDUSB

  Generates a  MIDI note when you push the button.
  Picks the note from a given scale each time.
  Changes scales when you push the scale button.

  Uses MIDIUSB for MIDI, so will work on any
  USB board (e.g. Uno, Leonardo, Micro, Yún, MKR, Nano33 IoT)

  Circuit:
      pushbutton attached to ground from pins 3 and 4.

  created 29 Mar 2020
  by Tom Igoe
*/
#include <MIDIUSB.h>
#include <PushButton.h>  // from https://github.com/tigoe/PushButton
#include <pitchToNote.h>

PushButton scaleButton(4);
PushButton playerButton(3);

// the intervals for various scales and modes:
byte scales[][7] = {
  {2, 2, 1, 2, 2, 2, 1},  // major
  {2, 1, 2, 2, 1, 2, 2},  // naturalMinor
  {2, 1, 2, 2, 1, 3, 1},  // harmonicMinor
  {2, 1, 2, 2, 2, 2, 1},  // melodicMinor
  {2, 2, 1, 2, 2, 2, 1},  // ionian
  {2, 1, 2, 2, 2, 1, 2},  // dorian
  {1, 2, 2, 2, 1, 2, 2},  // phyrgian
  {2, 2, 2, 1, 2, 2, 1},  // lydian
  {2, 2, 1, 2, 2, 1, 2},  // mixolydian
  {2, 1, 2, 2, 1, 2, 2},  // aeolian
  {1, 2, 2, 1, 2, 2, 2},  // locrian
  {3, 2, 1, 1, 3, 1, 0}   // blues
};

// an array to hold the final notes of the scale:
int scale[8];
// counter for the scales array:
int scaleCounter = 0;
// counter for the scale array:
int noteCounter = 0;
// start with middle C:
byte tonic = pitchG4;
// note to play:
byte noteValue = tonic;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  scaleButton.begin(INPUT_PULLUP);
  playerButton.begin(INPUT_PULLUP);

  // generate a scale from the tonic and the chosen interval:
  generateScale(scales[0], tonic);
  printScale(0);
}

void loop() {
  // read the pushbuttons:
  // if scale button is pressed, iterate through the scales:
  if (scaleButton.pressed()) {
    scaleCounter++;
    // constrain it to 0-11 range with rollover:
    scaleCounter = scaleCounter % 11;
    // generate a scale from the tonic and the chosen interval:
    generateScale(scales[scaleCounter], tonic);
    printScale(scaleCounter);
  }

  if (playerButton.changed()) {
    // if it's pressed:
    if (playerButton.read() == LOW) {
      // pick a random note in the scale:
      //      noteValue = scale[random(8)];

      // or traverse through the scale:
      noteValue = scale[noteCounter];
      noteCounter++;
      // constrain it to 0-8 range with rollover:
      noteCounter = noteCounter % 8;

      // play it:
      midiCommand(0x90, noteValue, 0x7F);
      midiCommand(0x91, noteValue, 0x7F);
//      midiCommand(0x9A, noteValue, 0x7F);
      // if it's not pressed:
    } else {
      // turn the note off:
      midiCommand(0x80, noteValue, 0);
      midiCommand(0x81, noteValue, 0x0);
//      midiCommand(0x8A, noteValue, 0x7F);
    }
  }
}

// generate a scale with the intervals and the tonic:

void generateScale(byte interval[], int tonic) {
  // fill the scale array with the scale you want:
  // start with the initial note:
  scale[0] = tonic;
  byte note = scale[0];
  // iterate over the intervals, adding each to the next note
  // in the scale:
  Serial.print("Notes: ");
  for (int n = 0; n < 7; n++) {
    note = note + interval[n];
    scale[n + 1] = note;
    Serial.print(scale[n]);
    Serial.print(" ");
  }
  Serial.println(scale[7]);
  Serial.println();
}

void printScale(int scaleNumber) {
  switch (scaleNumber) {
    case 0:   // major
      Serial.println("major");
      break;
    case 1:   // naturalMinor
      Serial.println("naturalMinor");
      break;
    case 2:   // harmonicMinor
      Serial.println("harmonicMinor");
      break;
    case 3:   // melodicMinor
      Serial.println("melodicMinor");
      break;
    case 4:   // ionian
      Serial.println("ionian");
      break;
    case 5:   // dorian
      Serial.println("dorian");
      break;
    case 6:   // phyrgian
      Serial.println("phyrgian");
      break;
    case 7:   // lydian
      Serial.println("lydian");
      break;
    case 8:   // mixolydian
      Serial.println("mixolydian");
      break;
    case 9:   // aeolian
      Serial.println("aeolian");
      break;
    case 10:   // locrian
      Serial.println("locrian");
      break;
    case 11:    //blues
      Serial.println("blues");
      break;
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
