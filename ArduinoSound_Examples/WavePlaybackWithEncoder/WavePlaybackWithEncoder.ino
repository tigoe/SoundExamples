/*
  This reads a wave file from an SD card and plays it using the I2S interface to
  a MAX08357 I2S Amp Breakout board.

  A pushbutton with pulldown resistor is connected
  to pin D7. Pressing it toggled play/pause
  An encoder is attached to pins 4 and 5. The encoder controls
  the volume of the sound.

  The wav file must be stereo signed 16 bit 44100Hz.

  Circuit:
   Arduino/Genuino Zero, MKRZero or MKR1000 board
   SD breakout or shield connected
   MAX08357:
     GND connected GND
     VIN connected 5V
     LRC connected to pin 0 (Zero) or pin 3 (MKR1000, MKRZero)
     BCLK connected to pin 1 (Zero) or pin 2 (MKR1000, MKRZero)
     DIN connected to pin 9 (Zero) or pin A6 (MKR1000, MKRZero)

  created 15 November 2016
  by Sandeep Mistry
  modified 29 Oct 2018
  by Tom Igoe
*/

#include <SD.h>
#include <ArduinoSound.h>
#include <Encoder.h>
Encoder volumeKnob(4, 5);

// filename of wave file to play
const char filename[] = "FELIZ.WAV";

// variable representing the Wave File
SDWaveFile waveFile;
// previous state of the pushbutton:
int lastButtonState = LOW;
int lastPosition = 0;
int loudness = 0;
void setup() {
  // Open serial communications:
  Serial.begin(9600);
  // make the button pin an input:
  pinMode(7, INPUT_PULLUP);
  // wait until serial monitor is open. This is
  // for debugging only. Remove when writing your own code:
  //while (!Serial);

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
    Serial.println("unable to play wave file using I2S!");
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
  // read the encoder knob position:
  int knobPosition = volumeKnob.read() ;
  // add the difference in position to loudness:
  loudness = loudness + (knobPosition - lastPosition);
  // constrain loudness to 0-100:
  loudness = constrain(loudness, 0, 100);
  if (lastPosition != knobPosition) {
    AudioOutI2S.volume(loudness);
  }
  // save current knob position for next comparison:
  lastPosition = knobPosition;

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
