NeoPixelParty
=============

NeoPixelParty (c) 2015 Tobias Theuer.
Licensed under LPGL3.
 
Requires [Adafruit NeoPixel Library](https://github.com/adafruit/Adafruit_NeoPixel).

Based on Adafruit's NeoPixel demo 'strandtest.ino' under LGPL3. 

## Building

Make sure you have the [Adafruit NeoPixel Library](https://github.com/adafruit/Adafruit_NeoPixel) in your Arduino IDE, clone this repository and click "Upload".

## Usage
Upload this sketch to an Arduino compatible board (we used a Adafruit Trinket) to which one or more Neopixels are
connected. We used a 16 pixel ring, for which this was optimized.

As soon as the sketch starts it will show random colorful patterns on the LEDs. It accepts no input and will run until
it loses power.

