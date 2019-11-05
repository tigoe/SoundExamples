#include <I2S.h>

int amplitude = 500;   // amplitude of square wave
int sampleRate = 8000; // sample rate in Hz
int count;

void setup() {
  Serial.begin(9600);
  // start I2S at the sample rate with 16-bits per sample
  while (!I2S.begin(I2S_PHILIPS_MODE, sampleRate, 16)) {
    Serial.println("I2S amp not responding.");
  }
}

void loop() {
  int sensorReading = analogRead(A0);
  // frequency is in Hz:
  //  float frequency = map(sensorReading, 0, 1023, 20, 13000);
  // current sample value:
  short sample = amplitude;
  float frequency = 0;
  Serial.println(frequency);
  // calculate half wavelength of a square wave:
  int halfWavelength = (sampleRate / frequency);

  if (count % halfWavelength == 0) {
    // invert the sample every half wavelength count multiple to generate square wave
    sample = -1 * sample;
  }

  // write the same sample twice, once for left and once for the right channel
  I2S.write(sample);
  I2S.write(sample);

  // increment the counter for the next sample
  count++;

}
