/*
  MIDI joystick controller

  Reads a joystick and when it has, sends
  MIDI noteon or noteoff  and pitch bend messages.

 
  Uses MIDIUSB for MIDI, so will work on any
  32U4-based board (e.g. Uno, Leonardo, Micro, Yún)

  Circuit:
    * Joystick pushbutton connected to pin 5
    * Joystick X axis connected to pin A0 

    created 19 Feb 2018
  modified 14 Jan 2019
  by Tom Igoe
*/

#include <MIDIUSB.h>
int baseNote = 48;            // C4, used as the base note
const int buttonPin = 5;
int lastButtonState = LOW;

void setup() {
  Serial.begin(9600);                  // initialize serial
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  byte note = baseNote;
  int x, y;
  // read the input on analog pin 0:
  int xSensor = analogRead(A0);
  delay(1);
  int ySensor = analogRead(A1);
  int buttonState = digitalRead(buttonPin);

  // map x and y readings to a 3-point range
  // and subtract 1 to get -1 to 1, with
  // 0 at rest:
  x = map(xSensor, 0, 1023, 0, 3) - 1;

  // x is the pitch bend axis:
  if (x != 0) {
    int pitchBendSensor = xSensor << 5;          // shift so top bit is bit 14
    byte msb = highByte(pitchBendSensor);        // get the high bits
    byte lsb = lowByte(pitchBendSensor >> 1);    // get the low 8 bits
    bitWrite(lsb, 7, 0);                         // clear the top bit of the low byte
    midiCommand(0xE0, lsb, msb);                 // send the pitch bend message
  }

  // button plays the note or stops it:
  if (buttonState != lastButtonState) {
    delay(5);     // debounce delay
    if (buttonState == LOW) {
      midiCommand(0x90, note, 127);
    } else {
      midiCommand(0x80, note, 0);
    }
  }
  lastButtonState = buttonState;

  // print the results
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");

  Serial.println(buttonState);
  // when all else fails, turn everything off:
//  midiCommand(0xB0, 0x7B, 0x00);
//  MidiUSB.flush();
  // Serial.println("all notes off");
}

void midiCommand(byte cmd, byte data1, byte  data2) {
  // First parameter is the event type (top 4 bits of the command byte).
  // Second parameter is command byte combined with the channel.
  // Third parameter is the first data byte
  // Fourth parameter second data byte

  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
}
