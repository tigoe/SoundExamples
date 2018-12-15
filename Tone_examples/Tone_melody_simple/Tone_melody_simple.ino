/*
  Simple Tone player

  Generates a series of tones from MIDI note values.
   The melody is Steve Reich's "Piano phase"

  Circuit:
  - 8 ohm speaker on digital pin 8

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
int thisNote = 0;

void setup() {

}

void loop() {

  // play a note from the melody:
  // convert MIDI note number to frequency
  // from https://en.wikipedia.org/wiki/MIDI_tuning_standard:
  float freq =  440 * pow(2, ((melody[thisNote] - 69) / 12.0));
  Serial.println(freq);

  // all the notes in this are sixteenth notes,
  // which is 1/4 of a beat, so:
  int noteDuration = beatDuration / 4;
  // turn the note on:
  tone(8, freq);
  // keep it on for the appropriate duration:
  delay(noteDuration);
  // turn the note off:
  noTone(8);

  // increment the note number for next time through the loop:
  thisNote++;
  // keep the note in the range from 0 - 12 using modulo:
  thisNote = thisNote % 12;
}
