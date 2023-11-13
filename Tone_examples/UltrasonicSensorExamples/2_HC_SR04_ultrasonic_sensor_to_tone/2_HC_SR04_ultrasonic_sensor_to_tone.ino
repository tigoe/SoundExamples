/*
 HC-SR04 distance-to-frequency converter
 This sketch reads an HC-SR04 ultrasonic distance sensor and converts
 any distance returned from 0-100cm into a frequency 
 between 110Hz (A2 on the equal tempered scale) 
 and 1046.5 Hz (C6 on the equal tempered scale). 

 There are many versions of this sketch on the internet, but 
 this version is based on the example at 
 https://itp.nyu.edu/physcomp/labs/lab-ultrasonic-distance-sensor/
 For more on the musical note conversion, see
 https://tigoe.github.io/SoundExamples/tone.html

 Created 12 Nov 2023
 Modified 13 Nov 2023
 by Tom Igoe
 */

// pin numbers for the connections to the sensor:
const int triggerPin = 9;
const int echoPin = 10;
const int speakerPin = A0;

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
  
  // map distance range (0-100cm) to frequency range (110-1046.5)
  float frequency = map(distance, 0, 100, 110, 1046.5);
  // turn the speaker on:
  tone(speakerPin, frequency, 300);
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