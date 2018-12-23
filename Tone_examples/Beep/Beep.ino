/*
  Plays a tone on a speaker at middle A (440Hz)

  circuit:
     * 8-ohm speaker and 100-ohm resistor
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
  // set the period in microseconds:
  int period = (1 / frequency) * 1000000;

  // turn the speaker on:
  digitalWrite(speakerPin, HIGH);
  // delay half the period:
  delayMicroseconds(period / 2);
  // turn the speaker off:
  digitalWrite(speakerPin, LOW);
  // delay half the period:
  delayMicroseconds(period / 2);
}
