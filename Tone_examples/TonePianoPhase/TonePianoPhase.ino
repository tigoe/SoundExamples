/*
  Simple Tone player

  Generates a series of tones from MIDI note values.
   The melody is Steve Reich's "Piano phase"

  Circuit:
  - 8 ohm speaker on digital pin 8

  created 13 Feb 2017
  modified 22 Dec 2018
  by Tom Igoe
*/
#define NOTE_A4 69         // MIDI note value for middle A
#define FREQ_A4 440        // frequency for middle A

const int speakerPin = 5;  // the pin number for the speaker

// the melody sequence:
int melody[] = {64, 66, 71, 73, 74, 66, 64, 73, 71, 66, 74, 73};
// which note of the melody to play:
int noteCounter = 0;

int bpm = 72;  // beats per minute
// duration of a beat in ms
float beatDuration = 60.0 / bpm * 1000;

void setup() {
  // make the speaker pin an output:
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  // current note is an element of the array:
  int currentNote = melody[noteCounter];
  // play a note from the melody:
  // convert MIDI note number to frequency:
  float frequency =  FREQ_A4 * pow(2, ((currentNote - NOTE_A4) / 12.0));

  // all the notes in this are sixteenth notes,
  // which is 1/4 of a beat, so:
  int noteDuration = beatDuration / 4;
  // turn the note on:
  tone(speakerPin, frequency);
  // keep it on for the appropriate duration:
  delay(noteDuration);
  // turn the note off:
  noTone(speakerPins);

  // increment the note number for next time through the loop:
  noteCounter++;
  // keep the note in the range from 0 - 11 using modulo:
  noteCounter = noteCounter % 12;
}
