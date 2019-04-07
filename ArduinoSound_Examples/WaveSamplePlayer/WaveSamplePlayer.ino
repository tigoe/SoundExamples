/*
  This sketch plays one of four .wav files when you press
  one of four corresponding pushbuttons.

  Pushbuttons are connected to pins 2 through 5. Their other sides
  are connected to ground. Pressing each starts playing a.wav file
  from the SD card. The sound files are called SOUND1.WAV through SOUND4.WAV

  The .wav file must be stereo signed 16 bit 44100Hz.

  Circuit:
   Arduino/Genuino Zero, MKRZero or MKR1000 board
   SD breakout or shield connected
   I2S amp:
     GND connected GND
     VIN connected Vcc
     LRC connected to pin 0 (Zero) or pin 3 (MKR1000, MKRZero)
     BCLK connected to pin 1 (Zero) or pin 2 (MKR1000, MKRZero)
     DIN connected to pin 9 (Zero) or pin A6 (MKR1000, MKRZero)
     pushbuttons:
     one side connected to pin 2 through 5
     the other connected to GND

  created 1 Apr 2019
  by Tom Igoe
*/

#include <SD.h>
#include <ArduinoSound.h>

// pushButton pin numbers:
const int pushButton[] = {0, 1, 4, 5};
// previous state of the pushbutton:
int lastButtonState[] = {1, 1, 1, 1};

// variable representing the Wave File
SDWaveFile waveFile;

void setup() {
  // Open serial communications:
  Serial.begin(9600);

  // set button pins as inputs:
  for (int b = 0; b < 4; b++) {
    // make the button pin an input:
    pinMode(pushButton[b], INPUT_PULLUP);
  }
  // wait until serial monitor is open:
  while (!Serial);

  // setup the SD card.
  // depending on your shield of breakout board
  // you may need to pass a pin number in begin for SS
  Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card is valid.");
  // set volume (0-100):
  AudioOutI2S.volume(20);
}

void loop() {
  // read button states:
  for (int b = 0; b < 4; b++) {
    // read the pin number for each button:
    int buttonState = digitalRead(pushButton[b]);
    // if it's changed:
    if (buttonState != lastButtonState[b] ) {
      delay(8);     // debounce delay
      // and if it's pressed
      if (buttonState == LOW) {
       // stop whatever is playing:
        if (AudioOutI2S.isPlaying()) AudioOutI2S.stop();
        // play the corresponding sample:
        String fileName = "SOUND" + String(b + 1) + ".WAV";
        if (!startNewFile(fileName)) {
          Serial.println("couldn't play " + fileName);
        }
      }
      // save button state for next comparison:
      lastButtonState[b] = buttonState;
    }
  }
}


int startNewFile(String thisFile) {
  /* result:
        -1: invalid file
        0: file can't play
        1: file is valid and can play
  */
  int result = -1;
  // create a SDWaveFile
  waveFile = SDWaveFile(thisFile);

  // check if the WaveFile is valid
  if (!waveFile) {
    Serial.println("wave file is invalid!");
    return result; // result = -1
  }
  result++;

  // check if the I2S output can play the wave file
  if (!AudioOutI2S.canPlay(waveFile)) {
    Serial.println("unable to play wave file using I2S!");
    return result; // result = 0
  }
  result++;

  // start playback
  Serial.println("playing file " + thisFile);
  AudioOutI2S.play(waveFile);
  return result;  // result = 1;
}
