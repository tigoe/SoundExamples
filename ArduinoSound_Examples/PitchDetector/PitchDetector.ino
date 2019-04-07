/*
  This example reads audio data from an I2S mic. It calculates the pitch range
  of an incoming sound. Peaks at about 4200 Hz, which is just above
  note value C8.  Then it calculates the nearest MIDI note number
  using the PitchToFrequency chart from the MIDIUSB library.

  Circuit:
   Arduino/Genuino Zero, MKRZero or MKR1000 board
   I2S mic:
     GND connected GND
     3.3V connected 3.3V (Zero) or VCC (MKR1000, MKRZero)
     WS connected to pin 0 (Zero) or pin 3 (MKR1000, MKRZero)
     CLK connected to pin 1 (Zero) or pin 2 (MKR1000, MKRZero)
     SD connected to pin 9 (Zero) or pin A6 (MKR1000, MKRZero)

  created 7 April 2019
  by Tom Igoe
  based on Sandeep Mistry's ArduinoSound examples
*/

#include <ArduinoSound.h>

// sample rate for the input, in Hz.
// to hear a max frequency, sample at 2 * the frequency.
// Check your mic to see what the top of its dynamic range is
const int sampleRate = 8400;

// size of the FFT to compute
const int fftSize = 128;

// size of the spectrum output, which is half of FFT size
const int spectrumSize = fftSize / 2;

// array to store spectrum output
int spectrum[spectrumSize];

// create an FFT analyzer to be used with the I2S input
FFTAnalyzer fftAnalyzer(fftSize);

// arbitrary threshold for loudness. Range is 0 - 2^32 (4,294,967,296)
int threshold = 100000;

void setup() {
  // setup the serial
  Serial.begin(9600);
  while (!Serial);
  // setup the I2S audio input for the sample rate with 32-bits per sample
  if (!AudioInI2S.begin(sampleRate, 32)) {
    Serial.println("Failed to initialize I2S input");
    while (true);
  }

  // configure the I2S input as the input for the FFT analyzer
  if (!fftAnalyzer.input(AudioInI2S)) {
    Serial.println("Failed to set FFT analyzer input");
    while (true);
  }
  
  // print the frequency bands:
  Serial.println("Frequency bands: ");
  for (int b = 0; b < spectrumSize; b++) {
    // if this sample is louder than your loudest so far:
    // get the low frequency for this band:
    int freqLow = (b * sampleRate) / fftSize;
    int freqHigh = ((b + 1) * sampleRate) / fftSize;
    Serial.print(freqLow);
    Serial.print(" - ");
    Serial.println(freqHigh);
  }
  delay(5000);
}

void loop() {
  if (fftAnalyzer.available()) {
    // analysis available, read in the spectrum
    fftAnalyzer.read(spectrum, spectrumSize);

    // analyze the bands of the FFT spectrum:
    int loudestSample = 0;  // the loudest sample in the FFT spectrum
    int freqLow = 0;        // low frequency of the loudest band
    int freqHigh = 0;       // high frequency of the loudest band
    // iterate over the spectrum:
    for (int b = 0; b < spectrumSize; b++) {
      // if this sample is louder than your loudest so far:
      if (spectrum[b] >= loudestSample) {
        // then this is the current loudest:
        loudestSample = spectrum[b];
        // get the low frequency for this band:
        freqLow = (b * sampleRate) / fftSize;
        // get the high frequency for this band:
        freqHigh = ((b+1) * sampleRate) / fftSize;
      }
    }

    // you're done. If your loudest was above a threshold,
    // print the results
    if (loudestSample > threshold) {
      Serial.print(freqLow);
      Serial.print(" ");
      Serial.print(freqHigh);
      Serial.print("    ");
      // print out the amplititude to the serial monitor
      Serial.println(loudestSample);
    }
  }
}
