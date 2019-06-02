/*
  Bluetooth LE MIDI example for the MKR1010

  Uses ArduinoBLE library

  created 22 Feb 2019
  by Tom Igoe
  Based on work by Don Coleman
*/
#include <ArduinoBLE.h>
byte midiData[] = {0x80, 0x80, 0x00, 0x00, 0x00};

int bpm = 72;  // beats per minute
// duration of a beat in ms
float beatDuration = 60.0 / bpm * 1000;

// the melody sequence:
int melody[] = {64, 66, 71, 73, 74, 66, 64, 73, 71, 66, 74, 73};
// which note of the melody to play:
int noteCounter = 0;

// set up the MIDI service and MIDI message characteristic:
BLEService midiService("03B80E5A-EDE8-4B33-A751-6CE34EC4C700");
BLECharacteristic midiCharacteristic("7772E5DB-3868-4112-A1A9-F2669D106BF3",
                                     BLEWrite | BLEWriteWithoutResponse |
                                     BLENotify | BLERead, sizeof(midiData));


void setup() {
  // initialize serial communication
  Serial.begin(9600);
  // initialize built in LED:
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize BLE:
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (true);
  }
  // set local name and advertised service for BLE:
  BLE.setLocalName("MIDI_1010");
  BLE.setAdvertisedService(midiService);

  // add the characteristic and service:
  midiService.addCharacteristic(midiCharacteristic);
  BLE.addService(midiService);

  // start advertising
  BLE.advertise();
}

void loop() {
  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    // turn on LED to indicate connection:
    digitalWrite(LED_BUILTIN, HIGH);
    // play a note from the melody:
    midiCommand(0x90, melody[noteCounter], 127);
    // all the notes in this are sixteenth notes,
    // which is 1/4 of a beat, so:
    int noteDuration = beatDuration / 4;
    // keep it on for the appropriate duration:
    delay(noteDuration);
    // turn the note off:
    midiCommand(0x80, melody[noteCounter], 127);
    // increment the note number for next time through the loop:
    noteCounter++;
    // keep the note in the range from 0 - 11 using modulo:
    noteCounter = noteCounter % 12;
  }
  // when the central disconnects, turn off the LED:
  digitalWrite(LED_BUILTIN, LOW);

}

// send a 3-byte midi message
void midiCommand(byte cmd, byte data1, byte  data2) {
  // MIDI data goes in the last three bytes of the midiData array:
  midiData[2] = cmd;
  midiData[3] = data1;
  midiData[4] = data2;

  midiCharacteristic.setValue(midiData, sizeof(midiData));
}
