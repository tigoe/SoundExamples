/*
  Bluetooth LE MIDI example for the MKR1010

  Uses ArduinoBLE library

  created 22 Feb 2019
  by Tom Igoe
  Based on work by Don Coleman
*/
#include <ArduinoBLE.h>

byte midiData[] = {0x80, 0x80, 0x00, 0x00, 0x00};

// the intervals in a major and natural minor scale:
int major[] = {2, 2, 1, 2, 2, 2, 1};
int naturalMinor[] = {2, 1, 2, 2, 1, 2, 2};

// an array to hold the final notes of the scale:
int scale[8];

// start with middle C:
int tonic = 65;
// note to play:
int noteValue = tonic;

// previous state of the button:
int lastButtonState = HIGH;


// set up the MIDI service and MIDI message characteristic:
BLEService midiService("03B80E5A-EDE8-4B33-A751-6CE34EC4C700");
BLECharacteristic midiCharacteristic("7772E5DB-3868-4112-A1A9-F2669D106BF3",
                                     BLEWrite | BLEWriteWithoutResponse |
                                     BLENotify | BLERead, sizeof(midiData));


void setup() {
  // initialize serial communication
  Serial.begin(9600);
  pinMode(5, INPUT_PULLUP);
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

  // fill the scale array with the scale you want:
  // start with the initial note:
  scale[0] = tonic;
  int note = scale[0];
  // iterate over the intervals, adding each to the next note
  // in the scale. You can change major to naturalMinor
  // if you want that kind of scale instead:
  for (int n = 0; n < 7; n++) {
    note = note + major[n];
    scale[n + 1] = note;
  }
}

void loop() {
  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    // turn on LED to indicate connection:
    digitalWrite(LED_BUILTIN, HIGH);
    while (central.connected()) {

      // read the pushbutton:
      int buttonState = digitalRead(5);
      Serial.println(buttonState);
      // compare its state to the previous state:
      if (buttonState != lastButtonState) {
        // debounce delay:
        delay(5);
        // if the button's changed and it's pressed:
        if (buttonState == LOW) {
          // pick a random note in the scale:
          noteValue = scale[random(8)];
          // play it:
          midiCommand(0x90, noteValue, 0x7F);
        } else  {
          // turn the note off:
          midiCommand(0x80, noteValue, 0);
        }
        // save the button state for comparison next time through:
        lastButtonState = buttonState;
      }
    }
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
