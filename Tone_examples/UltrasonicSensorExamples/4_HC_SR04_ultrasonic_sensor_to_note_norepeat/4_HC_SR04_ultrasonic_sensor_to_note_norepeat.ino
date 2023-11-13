/*
 HC-SR04 distance-to-equal-tempered-note converter
 This sketch reads an HC-SR04 ultrasonic distance sensor and converts
 any distance returned from 0-100cm into a note on the 
 equal-tempered western scale from A2 to C6. The sketch saves each 
 distance reading for comparison to the next reading, so that if there
 is no change in the reading, the note is not repeated.

 There are many versions of this sketch on the internet, but 
 this version is based on the example at 
 https://itp.nyu.edu/physcomp/labs/lab-ultrasonic-distance-sensor/
 For more on the musical note conversion, see
 https://tigoe.github.io/SoundExamples/tone.html

 created 12 Nov 2023
 modified 13 Nov 2023
 by Tom Igoe
 */

// middle A is the reference frequency for an
// equal-tempered scale. Set its frequency and note value:
#define NOTE_A4 69   // MIDI note value for middle A
#define FREQ_A4 440  // frequency for middle A
// pin numbers for the connections to the sensor:
const int triggerPin = 9;
const int echoPin = 10;
const int speakerPin = A0;

float lastDistance = 0;  // the previous distance read

void setup() {
  // set the functions of the pins.
  // the trigger pin needs to be an output:
  pinMode(triggerPin, OUTPUT);
  // the echo pin needs to be an input:
  pinMode(echoPin, INPUT);
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {
  // read the sensor and print the result:
  float distance = readSensor();
  Serial.print("Distance: ");
  Serial.println(distance);
  // ignore all readings greater than 100cm:
  if (distance > 100) return;
  // if the distance hasn't changed, skip the rest of the loop:
  if (distance == lastDistance) return;
  // convert sensor reading to the range of MIDI notes on an 88-key keyboard
  // from A2 to C6 (which is 45 - 84):
  float noteValue = map(distance, 0, 100, 45, 84);
  // then convert from MIDI note to frequency:
  float frequency = FREQ_A4 * pow(2, ((noteValue - NOTE_A4) / 12.0));
  // turn the speaker on:
  tone(speakerPin, frequency, 300);
  // save this sensor reading as lastSensorReading for comparison next time:
  lastDistance = distance;
  // delay 500 ms before playing again
  delay(500);
}

// this function reads the sensor and returns the reading
// in centimeters. The result is a float variable:
float readSensor() {
  float result = 0;
  float duration = 0;
  // set the trigger pin low, then high for 10 microseconds, then low.
  // this triggers the sensor to send out an ultrasonic pulse:
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  // listen for the pulse to return:
  duration = pulseIn(echoPin, HIGH);
  // calculate the distance in cm.
  //Sound travels approx.0.0343 microseconds per cm.,
  // and it's going to the target and back (hence the /2):
  result = (duration * 0.0343) / 2;
  // send the value back:
  return result;
}