  /*
  Plays a tone on a speaker from A0 to C8 
  (27.5 Hz to 4186). This is the range of notes 
  on an 88-key keyboard.
  The tone is set by a reading on pin A0

  circuit:
       8-ohm speaker and 100-ohm resistor
     in series on pin 5
       potentiometer connected to pin A0

   created 22 Dec 2018
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
