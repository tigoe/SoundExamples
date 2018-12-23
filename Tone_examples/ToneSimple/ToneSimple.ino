/*
  Plays a half-second note 
  and a half-second rest on a speaker at middle A (440Hz)

  circuit:
      8-ohm speaker and 100-ohm resistor
     in series on pin 5

   created 18 Dec 2018
   by Tom Igoe
*/

const int speakerPin = 5;  // the pin number for the speaker

void setup() {
  // make the speaker pin an output:
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  // set the frequency in Hz:
  float frequency = 440;
  // turn the speaker on:
  tone(speakerPin, frequency);
  delay(500);
  // turn the speaker off:
  noTone(speakerPin);
  delay(500);
}
