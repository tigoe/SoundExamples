/*
  One-key piano MIDI-over-OSC player for MIDUSB

  Generates a  MIDI notes when you push the button.
  Sends them over OSC to an OSCulator server.
  Randomizes the note within a scale each time

  Uses WiFi, so will work on the MKR1010, MKR1000, or Nano 33 IoT

  Circuit:
      pushbutton attached to +Vcc from pin 5.
      10-kilohm resistor to ground from pin 5

  created 20 Mar 2020
  by Tom Igoe
*/
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include "arduino_secrets.h"

WiFiUDP Udp;                 // instance of UDP library
const int remotePort = 8000; // port to which you want to send
char remoteAddress[] = "192.168.1.18";  //IP address you want to send to

const int buttonPin = 4;       // pin that the pushbutton is attached to

// the intervals in a major and natural minor scale:
int major[] = {2, 2, 1, 2, 2, 2, 1};
int naturalMinor[] = {2, 1, 2, 2, 1, 2, 2};

// an array to hold the final notes of the scale:
int scale[8];

// start with middle C:
int tonic = 0x3C; // pitchC4;
// note to play:
int noteValue = tonic;

// previous state of the button:
int lastButtonState = LOW;
// debounce delay:
int debounceDelay = 8;

void setup() {
  // initialize the button input:
  pinMode(buttonPin, INPUT_PULLUP);

  // open serial and wait for the serial monitor to open:
  Serial.begin(9600);
  while (!Serial);

  //   while you're not connected to a WiFi AP,
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println (SECRET_SSID);           // print the network name (SSID)
    WiFi.begin(SECRET_SSID, SECRET_PASS);   // try to connect
    delay(2000);
  }

  // When you're connected, print out the device's network status:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Udp.begin(remotePort);

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
  // read the pushbutton:
  int buttonState = digitalRead(buttonPin);
  // compare its state to the previous state:
  if (buttonState != lastButtonState) {
    // debounce delay:
    delay(debounceDelay);
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

// send a 3-byte midi message via OSC:
void midiCommand(byte cmd, byte data1, byte  data2) {
  /* Normally with MIDI,
    first parameter is command byte combined with the channel
    (e.g. note on channel 1 = 0x90)
    second parameter is the first data byte (for noteon/off, it's pitch)
    third parameter second data byte (for noteon/off, it's velocity)
  */
  // get the midi channel (bottom four bits of the command):
  byte midiChannel = cmd % 16;
  // get the command itself. This line assumes it's either 8 or 9,
  // i.e. noteoff (8) or noteon (9):
  byte midiCmd = (cmd >> 4) - 8;
  // make an OSC string:
  String oscString = "/midi/note/";
  // add the MIDI channel:
  oscString += midiChannel;
  // make a 3-byte array of the MIDI data:
  byte midimsg[] = {data1, data2, midiCmd};
  // print the String:
  Serial.println(oscString);
  // convert the String object to a char array
  // and make an OSC message with it:
  OSCMessage msg(oscString.c_str());
  // add the MIDI array to the OSC message and print it:
  for (int b = 0; b < sizeof(midimsg); b++) {
    // add each byte to the message:
    msg.add(midimsg[b]);
    // print it:
    Serial.print(midimsg[b], HEX);
    Serial.print(" ");
  }
  // print line at the end of the MIDI array:
  Serial.println();
  // start the UDP packet to send the OSC message:
  Udp.beginPacket(remoteAddress, remotePort);
  msg.send(Udp);      // send the bytes to the UDP stream
  Udp.endPacket();    // close the packet and send
  msg.empty();        // free space occupied by message
}
