/*
  MIDI melody player

  Generates a series of tones from MIDI note values.
   The melody is "Frere Jacques". Uses scheduler
   to play two voices in sync

  Circuit: none

  created 13 Feb 2017
  modified 23 Dec 2018
  by Tom Igoe
*/

#include <MIDIUSB.h>
#include <Scheduler.h>


#define NOTE_A4 69         // MIDI note value for middle A
#define FREQ_A4 440        // frequency for middle A

const int speakerPin = 5;  // the pin number for the speaker

// the melody sequence:
int melody[] = {1, 3, 5, 1,
                1, 3, 5, 1,
                5, 6, 8, 5, 6, 8,
                8, 10, 8, 6, 5, 1,
                8, 10, 8, 6, 5, 1,
                1, -4, 1,
                1, -4, 1, 0
               };
// the rhythm sequence:
int rhythm[] = {4, 4, 4, 4,
                4, 4, 4, 4,
                4, 4, 2,
                4, 4, 2,
                8, 8, 8, 8, 4, 4,
                8, 8, 8, 8, 4, 4,
                4, 4, 2,
                4, 4, 2, 1
               };
// which note of the melody to play:
int noteCounter[] = {0, 0};
int melodyLength = 33;

int bpm = 120;  // beats per minute
// duration of a beat in ms
float beatDuration = 60.0 / bpm * 1000;

int tonic = 65; // the tonic sets the key you're in

void setup() {
  delay(1000);
  Scheduler.startLoop(playSong);
  
  Serial.begin(9600);
}

void loop() {
  Serial.print(noteCounter[0]);
  Serial.print(" ");
  Serial.println(noteCounter[1]);
  // start the second voice on the 8th note of the first voice:
  if (noteCounter[0] == 8) {
    Scheduler.startLoop(playSong2);
  }
  // delay a beat, and yield the processor
  // for the other loops:
  delay(beatDuration);
  yield();
}

void playSong() {
  // current note is an element of the array:
  int currentCounter = noteCounter[0] % melodyLength;
  playNote(currentCounter);
  // increment the note number for next time through the loop:
  noteCounter[0]++;
  yield();
}

void playSong2() {
  int currentCounter = noteCounter[1] % melodyLength;
  playNote(currentCounter);
  // increment the note number for next time through the loop:
  noteCounter[1]++;
  yield();
}

void playNote(int counter) {
  int currentNote = melody[counter];
  // if the note is not a rest, add the tonic:
  if (currentNote != 0) {
    currentNote = currentNote + tonic;
  }

  // calculate noteDuration from the note type:
  float noteDuration = beatDuration * (4.0 / rhythm[counter]);

  // play a note from the melody if it's not a rest:
  if (currentNote > 0) {
    midiCommand(0x90, currentNote, 127);
  }
  // keep it on for the appropriate duration:
  delay(noteDuration);
  // turn the note off:
  midiCommand(0x90, currentNote, 0);
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
