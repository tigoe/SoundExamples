/*
  MIDI Controller change example

  Sends a control change for both the most significant byte
  and least significant bit (coarse and fine) for controllers
  0-31, by also sending a control change message for controllers
  32-64. For details, see https://www.midi.org/specifications-old/item/table-3-control-change-messages-data-bytes-2
  and https://www.midi.org/specifications-old/item/table-2-expanded-messages-list-status-bytes

  Uses MIDIUSB for MIDI, so will work on any
  32U4- or SAMD-based board (e.g. Nano 33 IoT, MKR Leonardo, Micro, Yún)

  created 16 April 2021
  by Tom Igoe
*/

#include <MIDIUSB.h>      // include MIDIUSB library
int controllerValue = 0;
int controllerNumber = 0;

void setup() {
  Serial.begin(9600);                  // initialize serial

}

void loop() {
// increment controller value up to 16383:
  controllerValue++;
  if (controllerValue == 16384) {
    controllerValue = 0;
    // increment controller number up to 31:
    controllerNumber++;
    if (controllerNumber == 32) {
      controllerNumber = 0;
    }
  }
// print controller value:
  Serial.println(controllerValue);

  int shiftedValue = controllerValue << 1;         // shift so top bit of lsb is in msb
  byte msb = highByte(shiftedValue);        // get the high bits
  byte lsb = lowByte(shiftedValue) >> 1;    // get the low 7 bits and shift right
  Serial.print("  ");
  Serial.print(msb, BIN);
  Serial.print("  ");
  Serial.println(lsb, BIN);

  // controllers 0 - 31 by themselves are 1-byte values
  // send the first control change with the MSB:
  midiCommand(0xB0, controllerNumber, msb);
  // but controllers 32-64 are the least significant byte
  // for controllers 0-31:
  // send the second control change with the LSB:
  midiCommand(0xB0, controllerNumber + 32, lsb);
  delay(1);
}

void midiCommand(byte cmd, byte data1, byte  data2) {
  // First parameter is the event type (top 4 bits of the command byte).
  // Second parameter is command byte combined with the channel.
  // Third parameter is the first data byte
  // Fourth parameter second data byte

  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
}
