/*
  Bluetooth LE MIDI example for the MKR1010

  Uses ArduinoBLE library

  Sends MIDI Control changes on channel 1, controllers 14 and 15. Useful for
  when you are using MIDI only, and want to send, say, X and Y values in controllers.

  created 18 April 2019
  by Tom Igoe
  Based on work by Don Coleman
*/
#include <ArduinoBLE.h>
byte midiData[] = {0x80, 0x80, 0x00, 0x00, 0x00};

// set up the MIDI service and MIDI message characteristic:
BLEService midiService("03B80E5A-EDE8-4B33-A751-6CE34EC4C700");
BLECharacteristic midiCharacteristic("7772E5DB-3868-4112-A1A9-F2669D106BF3",
                                     BLEWrite | BLEWriteWithoutResponse |
                                     BLENotify | BLERead, sizeof(midiData));

// previous value of the two controllers:
int lastController14Val = 0;
int lastController15Val = 0;

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
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);
    while (central.connected()) {
      // read analog in A0, A1, and convert to 7-bit range
      // by dividing by 8:
      int controller14Val = analogRead(A0) / 8;
      // delay to stabilize ADC:
      delay(1);
      int controller15Val = analogRead(A1) / 8;

      // send only if the values have changed:
      if (controller14Val != lastController14Val) {
        // send control change channel 1 (0xB0),controller 14 (0x0E), value:
        midiCommand(0xB0, 0x0E, controller14Val);
        lastController14Val = controller14Val;
      }
      if (controller15Val != lastController15Val) {
        // send control change channel 1 (0xB0),controller 15 (0x0F), value:
        midiCommand(0xB0, 0x0F, controller15Val);
        lastController15Val = controller15Val;
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
