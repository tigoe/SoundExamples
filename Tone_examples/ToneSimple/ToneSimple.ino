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

// middle A is the reference frequency for an
// equal-tempered scale. Set its frequency and note value:
#define NOTE_A4 69         // MIDI note value for middle A
#define FREQ_A4 440        // frequency for middle A

const int speakerPin = A0;  // the pin number for the speaker
void setup() {
  Serial.begin(9600);
  // make the speaker pin an output:
  pinMode(speakerPin, OUTPUT);
}
void loop() {
  // convert sensor reading to 21 - 108 range
  // which is the range of MIDI notes on an 88-key keyboard
  // (from A0 to C8):
  int sensorReading = analogRead(A1);
  int noteValue = map(sensorReading, 0, 1023, 21, 108);
  // then convert to frequency:
  float frequency =  FREQ_A4 * pow(2, ((noteValue - NOTE_A4) / 12.0));
  // turn the speaker on:
  tone(speakerPin, frequency, 300);
  Serial.println(frequency);
  delay(500);
}
