/*
  MIDI note pitch matcher
  Picks a random pitch and returns the nearest MIDI note to that pitch
  Written to use with pitch detectors, etc.

  Uses the pitchToFrequency table from the MIDUSB library:
  https://github.com/arduino-libraries/MIDIUSB/blob/master/src/pitchToFrequency.h

  created 29 Mar 2019
  by Tom Igoe

*/
#include <MIDIUSB.h>
#include <pitchToFrequency.h>

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // wait for Serial Monitor to open:
  while (!Serial);
}

void loop() {
  // pick a random pitch from 0 - 13000 Hz (above that is out of MIDI note range:
  int pitch = random (13000);
  // find the closest MIDI note corresponding to that pitch:
  int midiNote = findMidiNoteFromPitch(pitch);
  // print the results nicely:
  Serial.print(pitch);
  Serial.print( " is closest to MIDI note: ");
  Serial.print(midiNote);
  Serial.print(" whose pitch is: ");
  Serial.println(pitchFrequency[midiNote]);
  delay(1000);
}

int findMidiNoteFromPitch(int pitch) {
  // last difference; ultimately you want it small, so initialize it big:
  int lastDiff = 32000;
  // Your desired midi note; if it comes back -1, you know you had an error:
  int desiredMidiNote = -1;
  // iterate over all the known frequencies of MIDI notes:
  for (int x = 0; x < 127; x++) {
    // find the absolute value of difference between this pitch and your pitch:
    int diff = abs(pitch - pitchFrequency[x]);
    // if this is the smallest difference so far, it's the desired midi note:
    if (diff < lastDiff) {
      desiredMidiNote = x;
    }
    // save this difference for comparison next time through the for loop:
    lastDiff = diff;
  }
  // return the desired midi note:
  return desiredMidiNote;
}
