/*
  Plays a tone in response to a potentiometer

  circuit:
     * 8-ohm speaker and 100-ohm resistor
     in series on pin 5
     * Potentiometer connected to pin A0

   created 18 Dec 2018
   modified 22 Dec 2018
   by Tom Igoe
*/

const int speakerPin = 5;  // the pin number for the speaker

void setup() {
  // make the speaker pin an output:
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  // read the potentiometer:
  int sensorValue = analogRead(A0);
  // map the potentiometer to a range
  // from A0 to C8 (27.5 Hz to 4186):
  float frequency = map(sensorValue, 0, 1023, 27.5, 4186);
  // turn the speaker on:
  tone(speakerPin, frequency);
  delay(30);
}
