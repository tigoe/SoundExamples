/*
  MIDI player with Scheduler

  Plays a series of notes on a VS1053 audio chip.
  Uses the Scheduler library to play the melody in two parallel loops.
  This works with any of the shields or boards from
  Sparkfun or Adafruit that use the VS1053 chip.
  The melody is Steve Reich's "Piano phase"

  Uses Serial1 for MIDI, and the Scheduler library
  so it will work only on the MKR boards

  Circuit:
    connect TX of Serial1 to RX of the VS1053
    connect pin 6 to VS1053 reset pin

  created 13 Feb 2017
  modified 13 Dec 2018
  by Tom Igoe
*/
#include <Scheduler.h>

#define VS1053_RESET 6    // reset pin for the VS1053

// the melody sequence:
int melody[] = {64, 66, 71, 73, 74, 66, 64, 73, 71, 66, 74, 73};
int loops = 0;  // loop counter

void setup() {
  Serial.begin(9600);      // initialize serial
  Serial1.begin(31250);    // initialize MIDI serial

  // reset the VS1053 by taking reset low, then high:
  pinMode(VS1053_RESET, OUTPUT);
  digitalWrite(VS1053_RESET, LOW);
  delay(10);
  digitalWrite(VS1053_RESET, HIGH);
  // seed the random number generator with an analog reading:
  randomSeed(analogRead(A0));
  // start one loop:
  Scheduler.startLoop(piano1);
}

void loop() {
  delay(1000);

  // The second loop starts  after the first loop
  // has played once:
  if (loops == 1) {
    // start the other loop:
    Scheduler.startLoop(piano1);
  }
}

void piano1() {
  // iterate over the notes of the melody (12 notes):
  
  for (int thisNote = 0; thisNote < 12; thisNote++) {
    // set the tempo (random value):
    int tempo = random(70, 90);
    // play a note from the melody:
    midiCommand(0x90, melody[thisNote], 127);
    delay(tempo);
    // turn the note off:
    midiCommand(0x80, melody[thisNote], 127);
    // delay for slightly longer than the note length:
    delay(tempo * 1.2);
  }
  // give up processor control for the other loop:
  yield();
  loops++;
}

// send a 3-byte midi message
void midiCommand(byte cmd, byte data1, byte  data2) {
  Serial1.write(cmd);     // command byte (should be > 127)
  Serial1.write(data1);   // data byte 1 (should be < 128)
  Serial1.write(data2);   // data byte 2 (should be < 128)
}
