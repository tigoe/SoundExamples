/*
  MIDI reader

  reads MIDI input into the Serial1 RX pin.
  This is not a fullly implemented MIDI interpreter, just a stub.
  For details of each byte, see:
  https://www.midi.org/specifications-old/item/table-2-expanded-messages-list-status-bytes

  created 8 Mar 2019
  by Tom Igoe
*/


void setup() {
  // open Serial1 port for MIDI:
  Serial1.begin(31250);
  // open USB serial port for debugging:
  Serial.begin(9600);
}

void loop() {
  // if there's a new MIDI byte incoming:
  if (Serial1.available()) {
    // read it:
    char inByte = Serial1.read();
    // if it's a command byte (>= 0x80),
    // end the previous command:
    if (inByte >= 0x80) {
      Serial.println();
    }
    // print the byte in hexadecimal:
    Serial.print(inByte, HEX);
    Serial.print(" ");
  }
}
