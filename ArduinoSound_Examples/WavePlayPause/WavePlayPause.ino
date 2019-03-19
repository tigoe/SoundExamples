/*
  This reads a wave file from an SD card and plays it
  using the I2S interface to an I2S Amp Breakout board.
  It's been tested with the UDA1334A and the MAX98357 amps.

  The wav file must be stereo signed 16 bit 44100Hz.

  Circuit:
   * Arduino/Genuino Zero or MKR series board
   * SD breakout or shield connected. If using the MKRZero,
   the on-board SD card will work.
   * .wav file called SOUND.WAV on the card
   * pushbutton on the board connected to ground and pin 7. 
     Pressing it toggled play/pause
   * Amp wiring:
     GND connected GND
     VIN connected Vdd
     LRCLK (aka WSEL) connected to pin 3 (MKR1000, MKRZero) or pin 0 (Zero)
     BCLK connected to pin 2 (MKR1000, MKRZero) or pin 1 (Zero)
     DIN connected to pin A6 (MKR1000, MKRZero) or pin 9 (Zero)

  created 15 November 2016
  by Sandeep Mistry
  modified 29 Oct 2018
  by Tom Igoe
*/

#include <SD.h>
#include <ArduinoSound.h>

// filename of wave file to play
// file name must be 8chars . 3 chars
const char filename[] = "SOUND.WAV";

// variable representing the Wave File
SDWaveFile waveFile;
// previous state of the pushbutton:
int lastButtonState = HIGH;

void setup() {
  // Open serial communications:
  Serial.begin(9600);
  while (!Serial);
  // make the button pin an input, with internal pullup:
  pinMode(7, INPUT_PULLUP);

  // setup the SD card.
  // depending on your shield of breakout board
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
    Serial.println("unable to play wave file using I2S");
    while (1); // do nothing
  }

  // set the playback volume:
  AudioOutI2S.volume(20);
  // start playback
  Serial.println("looping file");
  AudioOutI2S.loop(waveFile);

}

void loop() {
  // read button state:
  int buttonState = digitalRead(7);
  // if it's changed:
  if (buttonState != lastButtonState ) {
    delay(8);     // debounce delay
    // if it's pressed
    if (buttonState == LOW) {
      // check whether audio is paused:
      if (AudioOutI2S.isPaused()) {
        // if it's paused, resume:
        AudioOutI2S.resume();
      } else {
        // if it's playing, pause it:
        AudioOutI2S.pause();
      }
      // print pause state:
      Serial.print("Audio paused: ");
      Serial.println(AudioOutI2S.isPaused());
    }
    // save button state for next comparison:
    lastButtonState = buttonState;
  }
}
