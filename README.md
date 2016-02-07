NeoPixelParty
=============

Shows colorful blinking patterns on a strip or - ideally - ring of WS281x LEDs (aka NeoPixel).

NeoPixelParty (c) 2015 Tobias Theuer.
Licensed under LPGL3.
 
Requires [Adafruit NeoPixel Library](https://github.com/adafruit/Adafruit_NeoPixel).

Based on Adafruit's NeoPixel demo 'strandtest.ino' under LGPL3. 

## Usage
Upload this sketch to an Arduino compatible board (we used a Adafruit Trinket) to which one or more Neopixels are
connected. We used a 16 pixel ring, for which this was optimized.

As soon as the sketch starts it will show random colorful patterns on the LEDs. It accepts no input and will run until
it loses power.

## Configuration

Edit the `#define`s in the `CONSTANTS` section of the source code.
Specifically of interest might be `STRIP_LENGTH`, which is the number of LEDs, and `PATTERN_LENGTH_MS`, which is how many milliseconds a pattern is active before the next one is called.

## Building

Make sure you have the [Adafruit NeoPixel Library](https://github.com/adafruit/Adafruit_NeoPixel) in your Arduino IDE, clone and open this repository and click "Upload".
