void setup() {
  Serial.begin(9600);
  while (!Serial);
}

void loop() {
  // iterate to 14 bits:
  for (int myNumber = 0; myNumber < 16384; myNumber++) {
    // if only using 10 bits, map to 14:
    //    int shiftedValue = map(myNumber, 0, 1023, 0, 16383);
    int shiftedValue = myNumber << 1;         // shift to get top bit of lsb in msb

    // alternative: shift whole number up for max sensitivity:
    //    int shiftedValue = myNumber << 5;   // shift to get top bits of lsb in msb

    // separate the bytes:
    byte msb = highByte(shiftedValue);        // get the high bits
    byte lsb = lowByte(shiftedValue) >> 1;    // get the low 8 bits
    Serial.print(myNumber);  // print  original number
    Serial.print(" ");
    Serial.print(msb, BIN);  // print msb in binary
    Serial.print("   ");
    Serial.println(lsb, BIN);// print lsb in binary

    // check for a 1 in the top bit of either byte:
    if (msb > 127 || lsb > 127) {
      Serial.print("problem: ");
      Serial.print(myNumber);
      Serial.print(" ");
      Serial.print(msb);
      Serial.print(" ");
      Serial.println(lsb);
      // stop:
      while (1);
    }
  }
  Serial.println("all good");
  // stop:
  while (1);
}
