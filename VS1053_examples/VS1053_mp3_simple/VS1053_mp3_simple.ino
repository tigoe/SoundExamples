/*
  VS1053 example
  using SparkFun MP3 shield
  and Adafruit VS1053 library
  in beatiful harmony

  A potentiometer controls the volume.

  circuit:
    VS1053 module attached to the pins described below
    Potentiometer attached to pin A0

   note: on the MKR Zero, use the SD card on the MKR rather than
   one on the sound player module. To do this, set CARDCS to SDCARD_SS_PIN

  created 30 Nov 2018
  modified 17 Oct 2024
  by Tom Igoe

*/
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

// the VS1053 chip and SD card are both SPI devices.
// Set their respective pins:

#define CLK 13          // SPI Clock, shared with SD card
#define MISO 12         // Input data, from VS1053/SD card
#define MOSI 11         // Output data, to VS1053/SD card
#define VS1053_RESET 6  // VS1053 reset pin
#define VS1053_CS 7     // VS1053 chip select pin
#define VS1053_DCS 4    // VS1053 Data/command select pin
#define CARDCS 3        // SD card chip select pin
#define VS1053_DREQ 5   // VS1053 Data request

// make an instance of the MP3 player library:
Adafruit_VS1053_FilePlayer mp3Player =
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

// sound file name must be 8 chars .3 chars and must be the absolute path:
const char soundFile[] = "/track001.mp3";

void setup() {
  Serial.begin(9600);
  // wait for serial port to open
  if (!Serial) delay(3000);
  // reset the VS1053 by taking reset low, then high:
  pinMode(VS1053_RESET, OUTPUT);
  digitalWrite(VS1053_RESET, LOW);
  delay(10);
  digitalWrite(VS1053_RESET, HIGH);

  // initialize the MP3 player:
  if (!mp3Player.begin()) {
    Serial.println("VS1053 not responding. Check to see if the pin numbers are correct.");
    while (true)
      ;  // stop
  }

  // initialize the SD card on the module:
  if (!SD.begin(CARDCS)) {
    Serial.println("SD failed, or not present");
    while (true)
      ;  // stop
  }

  // Set volume for left and right channels.
  // 0 = loudest, 100 = silent:
  mp3Player.setVolume(10, 10);

  // use the VS1053 interrrupt pin so it can
  // let you know when it's ready for commands.
  if (!mp3Player.useInterrupt(VS1053_FILEPLAYER_PIN_INT))
    Serial.println("DREQ pin is not an interrupt pin");

  // play file:
  mp3Player.playFullFile(soundFile);
  mp3Player.startPlayingFile(soundFile);
  Serial.println("playing");
}
int lastLoudness = 0;
void loop() {
  // read a potentiometer (0-1023 readings) and
  // map to a range from 100 to 0:
  int loudness = map(analogRead(A0), 0, 1023, 100, 0);
  // set the volume:
  mp3Player.setVolume(loudness, loudness);

  // loop the player:
  if (mp3Player.stopped()) {
    Serial.println("repeating");
    mp3Player.startPlayingFile(soundFile);
  }
}
