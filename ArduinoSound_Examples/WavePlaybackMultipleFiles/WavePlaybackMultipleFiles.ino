/*
  This reads a list of  .wav files from an SD card
  and plays the first one it using the I2S interface
  to an I2S Amp Breakout board.

  A pushbutton is connected to pin D7. The other side is connected to ground.
  Pressing it starts the next .wav file on the SD card.
  A potentiometer is attached to pins A0. It controls
  the volume of the sound.

  The .wav file must be stereo signed 16 bit 44100Hz.

  Circuit:
   Arduino/Genuino Zero, MKRZero or MKR1000 board
   SD breakout or shield connected
   I2S amp:
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
   potentiometer:
     wiper to pin A0
     ends to Vcc and GND
   pushbutton:
     one side connected to pin 7
     the other connected to GND

  created 31 Mar 2019
  updated 23 Sep 2021
  by Tom Igoe
*/

#include <SD.h>
#include <ArduinoSound.h>
#define I2S_DEVICE 1    // Nano 33 IoT needs I2S activated

// filename of wave file to play
String fileName;

// variable representing the Wave File
SDWaveFile waveFile;

// previous state of the pushbutton:
int lastButtonState = LOW;
int currentFile = 1;

void setup() {
  // Open serial communications:
  Serial.begin(9600);
  // make the button pin an input:
  pinMode(7, INPUT_PULLUP);
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
  String fileName = getFileList(currentFile);
  // start a new WAV file:
  if (startNewFile(fileName)) {
    // start with a low volume:
    AudioOutI2S.volume(100);
  }
}

void loop() {
  // read loudness knob:
  int knobReading = analogRead(A0);
  // map reading to get loudness:
  float loudness  = map(knobReading, 0, 1023, 0, 100);
   AudioOutI2S.volume(loudness);

  // read button state:
  int buttonState = digitalRead(7);
  // read the potentiometer knob position:

  // if it's changed:
  if (buttonState != lastButtonState ) {
    delay(8);     // debounce delay
    // if it's pressed
    if (buttonState == LOW) {
      currentFile++;
       // stop the previous file if playing
       // you must do this before you get the file list
       // from the card again:
         if (AudioOutI2S.isPlaying()) AudioOutI2S.stop();
      String fileName = getFileList(currentFile);
      if (fileName != "") {
       
        //start a new WAV file:
        if (!startNewFile(fileName)) {
          Serial.println("No available WAV files to play");
        }
      } else { // you're at the end of the file list, reset:
        currentFile = 0;
      }
    }
    // save button state for next comparison:
    lastButtonState = buttonState;
  }
}

String getFileList(int currentFile) {
  int waveFileCount = 0;
  String result;

  File root = SD.open("/");
  File entry;
  while (waveFileCount < currentFile) {
    entry = root.openNextFile();
    if (!entry) {
      result = "";
      break;
    }
    if (!entry.isDirectory()) {
      result = String(entry.name());
      if (result.endsWith(".WAV")) {
        waveFileCount++;
      }
    }
    entry.close();
  }
  // now the fileName is the current file,
  // or it's empty
  return result;
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
  // print the file's name and duration:
  Serial.println("File name: " + thisFile);
  long duration = waveFile.duration();
  Serial.print("   duration = ");
  Serial.print(duration);
  Serial.println(" seconds");

  // check if the I2S output can play the wave file
  if (!AudioOutI2S.canPlay(waveFile)) {
    Serial.println("unable to play wave file using I2S!");
    return result; // result = 0
  }
  result++;

  // start playback
  Serial.println("playing file");
  AudioOutI2S.play(waveFile);
  return result;  // result = 1;
}
