/*
  Simple MIDI player for VS1053

  Plays a series of notes on a VS1053 audio chip.
  This works with any of the shields or boards from
  Sparkfun or Adafruit that use the VS1053 chip.
  The melody is Steve Reich's "Piano phase"

  Uses Serial1 for MIDI, so will work on any board
  with 2 hardware serial ports: MKR boards, Leonardo, Micro, or Yún

  Circuit:
    connect TX of Serial1 to RX of the VS1053
    connect pin 6 to VS1053 reset pin

  created 13 Feb 2017
  modified 13 Dec 2018
  by Tom Igoe
*/

#define VS1053_RESET 6    // reset pin for the VS1053

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

  // reset the VS1053 by taking reset low, then high:
  pinMode(VS1053_RESET, OUTPUT);
  digitalWrite(VS1053_RESET, LOW);
  delay(10);
  digitalWrite(VS1053_RESET, HIGH);
}

void loop() {
  // play a note from the melody:
  midiCommand(0x90, melody[noteCounter], 127);
  // all the notes in this are sixteenth notes, 
  // which is 1/4 of a beat, so:
  int noteDuration = beatDuration / 4;
  // I like when the note is on 60% of the note time:
  delay(noteDuration);
  // turn the note off:
  midiCommand(0x80, melody[noteCounter], 127);
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
