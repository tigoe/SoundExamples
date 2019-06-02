/*
  Simple MIDI player for Serial1

  Generates a series of MIDI notes.
   The melody is Steve Reich's "Piano phase"

  Uses Serial1 for MIDI, so will work on any board
  with 2 hardware serial ports: MKR boards, Leonardo, Micro, or Yún

  Circuit:
    connect TX of Serial1 to TX pin of MIDI jack

  created 13 Feb 2017
  modified 13 Dec 2018
  by Tom Igoe
*/

int bpm = 72;  // beats per minute
// duration of a beat in ms
float beatDuration = 60.0 / bpm * 1000;

// the melody sequence:
int melody[] = {64, 66, 71, 73, 74, 66, 64, 73, 71, 66, 74, 73};
// which note of the melody to play:
int noteCounter = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // initialize MIDI serial:
  Serial1.begin(31250);
}

void loop() {
  // play a note from the melody:
  midiCommand(0x90, melody[noteCounter], 127);
  // all the notes in this are sixteenth notes,
  // which is 1/4 of a beat, so:
  int noteDuration = beatDuration / 4;
  // keep it on for the appropriate duration:  delay(noteDuration);
  // turn the note off:
  midiCommand(0x90, melody[noteCounter], 0);
  // increment the note number for next time through the loop:
  noteCounter++;
  // keep the note in the range from 0 - 11 using modulo:
  noteCounter = noteCounter % 12;
}

// send a 3-byte midi message
void midiCommand(byte cmd, byte data1, byte  data2) {
  Serial1.write(cmd);     // command byte (should be > 127)
  Serial1.write(data1);   // data byte 1 (should be < 128)
  Serial1.write(data2);   // data byte 2 (should be < 128)
}
