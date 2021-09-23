/*
  This reads a wave file from an SD card and plays it
  using the I2S interface to an I2S Amp Breakout board.
  It's been tested with the UDA1334A and the MAX98357 amps.

  The wav file must be stereo signed 16 bit 44100Hz.

  Circuit:
   * Arduino/Genuino Zero or MKR series board
   * SD breakout or shield connected. If using the MKRZero,
   the on-board SD card will work.
   * A potentiometer connected to pin A0 adjusts the volume
   * Amp wiring:
     GND connected GND
     VIN connected Vdd
     LRCLK (aka WSEL) connected to:
        - pin 3 (MKR1000, MKRZero)
        - pin 0 (Zero)
        - pin A2 (Nano 33 IoT)
     BCLK connected to:
        - pin 2 (MKR1000, MKRZero)
        - pin 1 (Zero)
        - pin A3 (Nano 33 IoT)
     DIN connected to:
        - pin A6 (MKR1000, MKRZero)
        - pin 9 (Zero)
        - pin 4 (Nano 33 IoT)

  created 15 November 2016
  by Sandeep Mistry
  modified 23 Sep 2021
  by Tom Igoe
*/

#include <SD.h>
#include <ArduinoSound.h>
#define I2S_DEVICE 1    // Nano 33 IoT needs I2S activated

// filename of wave file to play
// file name must be 8chars . 3 chars
const char filename[] = "SOUND.WAV";

// variable representing the Wave File
SDWaveFile waveFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
 
  // setup the SD card, depending on your shield of breakout board
  // you may need to pass a pin number in begin for SS
  Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card is valid.");

  // create a SDWaveFile
  waveFile = SDWaveFile(filename);

  // check if the WaveFile is valid
  if (!waveFile) {
    Serial.println("wave file is invalid!");
    while (1); // do nothing
  }
  // print the file's duration:
  long duration = waveFile.duration();
  Serial.print("Duration = ");
  Serial.print(duration);
  Serial.println(" seconds");

  // check if the I2S output can play the wave file
  if (!AudioOutI2S.canPlay(waveFile)) {
    Serial.println("unable to play wave file using I2S!");
    while (1); // do nothing
  }

  // start playback
  Serial.println("looping file");
  AudioOutI2S.loop(waveFile);
}

void loop() {
  // read analog in
  int sensorReading = analogRead(A0);
  // map to a 0-100 range:
  int loudness = map(sensorReading, 0, 1023, 0, 100);
  // set the volume:
  AudioOutI2S.volume(loudness);
  Serial.print("loudness: ");
  Serial.println(loudness);
}
