/*
  MIDI file player for VS1053

  Plays a MIDI file from an SD card on the VS1053 chip.
  This works with any of the shields or boards from
  Sparkfun or Adafruit that use the VS1053 chip.

  Uses Serial1 for MIDI, so will work on any board
  with 2 hardware serial ports: MKR boards, Leonardo, Micro, or Yún

  Circuit:
     connect TX of Serial1 to RX of the VS1053
     connect pin 6 to VS1053 reset pin
     SD card connected to SPI port

  created 14 Dec 2018
  by Tom Igoe
*/
#include <SPI.h>
#include <SD.h>

#define VS1053_RESET 6    // reset pin for the VS1053
#define CARDCS SDCARD_SS_PIN
const char fileName[] = "midifile.mid";

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // initialize MIDI serial:
  Serial1.begin(31250);

  // reset the VS1053 by taking reset low, then high:
  pinMode(VS1053_RESET, OUTPUT);
  digitalWrite(VS1053_RESET, LOW);
  delay(10);
  digitalWrite(VS1053_RESET, HIGH);

  if (!SD.begin(CARDCS)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  File midiFile = SD.open(fileName);

  if (midiFile) {
    while (midiFile.available()) {
      Serial1.write(midiFile.read());
    }
    midiFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.print("error opening ");
    Serial.println(fileName);
  }

}

void loop() {
}
