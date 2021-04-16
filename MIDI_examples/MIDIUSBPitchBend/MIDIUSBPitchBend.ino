/*
  MIDI pitch bend example

  plays a noteOn every two seconds, then uses a potentiometer
  to bend the pitch

  Uses MIDIUSB for MIDI, so will work on any
  32U4- or SAMD-based board (e.g. Nano 33 IoT, MKR Leonardo, Micro, Yún)

  Circuit:
      potentiometer connected to pin A0

  created 17 Feb 2019
  by Tom Igoe
*/

#include <MIDIUSB.h>      // include MIDIUSB library
#include <pitchToNote.h>  // include pitch chart from MIDIUSB library

void setup() {
  Serial.begin(9600);                  // initialize serial
}

void loop() {
  // play a note every two seconds:
  if (millis() % 2000 < 2) {
    midiCommand(0x80, pitchC4, 0);
    midiCommand(0x90, pitchC4, 64);
  }

  int sensor = analogRead(A0);

  // pitch bend range is 0 - 16383, and
  // the middle of the range, 8192, is zero pitch bend:
  int pitchBend = map(sensor, 0, 1023, 0, 16383);
  Serial.print(pitchBend);

  // convert 8-bit bytes to 7-bit bytes:
  int shiftedValue = pitchBend << 1;        // shift so top bit of lsb is in msb
  byte msb = highByte(shiftedValue);        // get the high bits
  byte lsb = lowByte(shiftedValue) >> 1;    // get the low 7 bits and shift right

  // print the values, for reference:
  Serial.print(" ");
  Serial.print(msb);
  Serial.print(" ");
  Serial.println(lsb);


  // send the pitch bend message:
  midiCommand(0xE0, lsb, msb);
}

void midiCommand(byte cmd, byte data1, byte  data2) {
  // First parameter is the event type (top 4 bits of the command byte).
  // Second parameter is command byte combined with the channel.
  // Third parameter is the first data byte
  // Fourth parameter second data byte

  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
}
