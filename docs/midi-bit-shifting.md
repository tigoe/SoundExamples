# Converting Bytes to MIDI Bytes

MIDI represents all values in 7-bit bytes, but most digital devices use 8-bit bytes. So when you want to convert a value from another device, like a laptop or a microcontroller, you need to know a bit about how the numbers are stored in computer memory.

Computers store numbers as *binary* or base 2 values, called *bits*. Most computers store them in chunks of 8 bits called *bytes*. So a number like 1023 would take up 10 bits of space in a computer's memory, and would be stored in two bytes, like so:

1023 = 00000011 11111111

The left-hand byte above is called the *most significant byte* or *high byte*, and the right-hand byte is the *least significant byte* or *low byte*. Likewise, the leftmost bit of each bit is the *most significant bit* and the rightmost is the *least significant bit*. If this is confusing, think back to base 10: the leftmost digit in the number 1023 is the most significant, because it represents 1000, while the rightmost digit is the least significant, since it represents only 3. 

Now, let's imagine that you want to convert an analog sensor reading from a microcontroller like Arduino into a MIDI parameter like pitch bend. For example, maybe you're using a force sensing resistor as your pitch bend. The Arduino's analog inputs, like many microcontrollers, convert incoming voltage into a 10-bit value. So you would need to move some bits around to get the right values to send as a pitch bend message. Following are two ways to do this.

## Converting 8-bit Numbers to 7-bit Numbers

Let's start with a general conversion from 8-bit to 7-bit. Let's assume that the number you want to convert is no larger than 2<sup>14</sup>, or 14 bits. That's 16383 in decimal notation. Why no more than 2<sup>14</sup>? Because 2 7-bit bytes can't hold a number larger than that.

First, list the whole number as a 16-bit (or two-byte) array (a space has been added between the bytes for readability):

16383 base 10 = 00111111 11111111 in binary

You can't keep the 1 value in the most significant bit of the low byte. So shift the whole value to the left by 1 like so:

01111111 11111110

To do this shifting in a program, you'd use the shift-left operator, which looks like this: `<<`. To shift right, you'd use the shift-right operator: `>>`

Now you've saved that 1 in the most significant byte. Next, shift the low byte to the right by one to clear that top bit:

01111111 01111111

That's it! Now you've got the whole original value represented in two 7-bit bytes.

The Arduino code to do this is as follows:

````
    int myNumber = 16383;
    int shiftedValue = myNumber << 1;         // shift so top bit of lsb is in msb
    byte msb = highByte(shiftedValue);        // get the high bits
    byte lsb = lowByte(shiftedValue) >> 1;    // get the low 7 bits and shift right
````

You can use this formula for any number, just plug in your own number. For example, a pitch bend, has the command byte value 0xE0 for channel 0. The full pitch bend on channel 0 would be:
````
0xE0,msb,lsb
````

# Mapping 10-bit Values to 2 7-Bit MIDI Values

The formula above converts 8-bit to 7-bit values faithfully, but what if you wanted to map the range of your sensor onto the range of your pitch bend? You could simply insert the `map()` function like so:

````
    int myNumber = 1023;
    int mappedValue = map(myNumber, 0, 1023, 0, 16383); 
    int shiftedValue = mappedValue << 1;      // shift so top bit of lsb is in msb
    byte msb = highByte(shiftedValue);        // get the high bits
    byte lsb = lowByte(shiftedValue) >> 1;    // get the low 7 bits and shift right
````

This works reasonably well, but sometimes you want the maximum sensitivity. In other words, make it so the highest sensor value corresponds to the highest pitch bend value, and you don't care about the low end of the values.  To do this, you need to push your sensor reading's most significant bits into the most significant 7 bits of the high byte, and put the rest in the most significant bits of the low byte. Here's how to do that:

First, list the whole number as a 16-bit (or two-byte) array again. Since it's an analog input reading, the highest possible value is 1023:

 1023 base 10 = 00000011 11111111 

Now, you want to fit this into two 7-bit bytes. So put the seven most significant bits into the most significant byte by shifting them by five places:

00000011 11111111 >> 5 = 01111111 11100000

Now you need the three least significant bits in a 7-bit byte of their own. Shift them right by one:

11100000 >> 1 = 01110000

The result is two bytes, each only 7 bits, that contain your pitch bend number. This results in a pitch bend value that's very responsive to your sensor. However, you can never really get a very low pitch bend with this. If you care about subtle pitch bends, then the original formula shown above is the better one to use. If you care about hearing a wide pitch bend range, then use this formula. Here's the Arduino code to do this mapping:

 ````   
    int sensor = analogRead(A0);
    int pitchBend = sensor << 5;           // shift so top bit is bit 14
    byte msb = highByte(pitchBend);        // get the high bits
    byte lsb = lowByte(pitchBend) >> 1;    // get the low 8 bits
````

Note: If you want to make sure your resulting values are always 7-bit, you can clear the top bit like so:

````
    bitWrite(msb, 7, 0);   // clear the top bit of the high byte
    bitWrite(lsb, 7, 0);   // clear the top bit of the low byte
````

You shouldn't need this though, as the formulas above should always result in legitimate 7-bit values.

If you'd like some code to try this, check out the [BitConverterUnitTest](https://github.com/tigoe/SoundExamples/blob/master/MIDI_examples/BitConverterUnitTest/BitConverterUnitTest.ino) example.