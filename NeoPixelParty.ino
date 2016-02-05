/*-------------------------------------------------------------------------
  NeoPixelParty (c) 2015 Tobias Theuer <theuer@hardbop.org>.
  
  Based on Adafruit's NeoPixel demo 'strandtest.ino' under LGPL3.
  Using Adafruit NeoPixel Library under LGPL3.
  <https://github.com/adafruit/Adafruit_NeoPixel>

  -------------------------------------------------------------------------
  
  NeoPixelParty is free software: you can redistribute it and/or 
  modify it under the terms of the GNU Lesser General Public License
  as published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoPixel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoPixel.  If not, see
  <http://www.gnu.org/licenses/>.
  -------------------------------------------------------------------------*/


/******************
**** LIBRARIES ****
******************/

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
    #include <avr/power.h>
#endif

/******************
**** CONSTANTS ****
******************/

// Uncomment (Ctrl + / in Arduino IDE or CLion) to set brightness to lower levels and only run the last pattern.
//#define DEBUG

// What pin is the strips data line connected to.
#define PIN 1

// How many LEDs are on the strip.
#define STRIP_LENGTH 16

// How many different patters there are.
#define NUMBER_OF_PATTERNS 8

// How long a pattern is active before jumping to the next. In milliseconds.
#define PATTERN_LENGTH_MS 15000

// How long a single animation frame is. In milliseconds.
#define FRAME_LENGTH_MS 66

// Macros for the timer function.
#define START_TIMER() miliseconds = millis();
#define CHECK_TIMER() if (millis() > miliseconds + PATTERN_LENGTH_MS) return;

/*************************
**** GLOBAL VARIABLES ****
*************************/

// The LED strip.
/* Parameter 1 = number of pixels in strip
   Parameter 2 = Arduino pin number (most are valid)
   Parameter 3 = pixel type flags, add together as needed:
       NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
       NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
       NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
       NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2) */
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, PIN, NEO_GRB + NEO_KHZ800);

// Timer variable
unsigned long miliseconds = 0;

// Pattern variables.
long pattern = 0;
long old_pattern = 1;

/**************
**** SETUP ****
**************/

void setup() {
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // End of trinket special code

    strip.begin();
    strip.show();
}

/*************
**** LOOP ****
*************/

void loop() {
    // (Re)Set brightness to default (half of max)
    strip.setBrightness(128);

#ifdef DEBUG
    // Set brigthness to less blinding if in debug mode
    strip.setBrightness(32);
#endif

    // Set all pixels to black
    for (byte i = 0; i < STRIP_LENGTH; i++) {
        strip.setPixelColor(i, 0);
    }


    old_pattern = pattern;

    // Get a random number
    pattern = random(NUMBER_OF_PATTERNS);

    // Avoid having the same pattern twice in a row
    if (pattern == old_pattern) return;

#ifdef DEBUG
    // Set the pattern to the last one if in debug mode.
    pattern = NUMBER_OF_PATTERNS - 1;
#endif

    // Choose the pattern that corresponds to the (random) number
    switch (pattern) {
        case 0:
            theaterChase(Wheel(random(256)), random(2));
            break;
        case 1:
            rainbow(random(2) == 0);
            break;
        case 2:
            theaterChaseReverse(Wheel(random(256)), random(2));
            break;
        case 3:
            chasingRainbows();
            break;
        case 4:
            flash();
            break;
        case 5:
            bubble();
            break;
        case 6:
            chasingRainbowsReverse();
            break;
        case 7:
            blinkRandom();
    }
}

void setPixel(byte pos, uint32_t col) {
    if (pos > STRIP_LENGTH - 1) pos = pos % STRIP_LENGTH;
    if (pos < 0) pos = pos + STRIP_LENGTH;
    strip.setPixelColor(pos, col);
}

void blinkRandom() {
    START_TIMER();

    strip.setBrightness(64);

    while (true) {


        for (byte j = 0; j < 256; j++) {
            uint32_t color = Wheel(j);

            for (byte i = 0; i < STRIP_LENGTH; i++) {
                strip.setPixelColor(i, color);
            }

            strip.show();

            delay(FRAME_LENGTH_MS / 3);

            CHECK_TIMER();
        }
    }
}

void rainbow(bool dir) {
    START_TIMER();

    byte pos = 0;

    for (byte j = random(256); j < 256; j++) {
        strip.setPixelColor(pos, Wheel(j));

        strip.show();

        CHECK_TIMER();

        delay(FRAME_LENGTH_MS);
        strip.setPixelColor(pos, 0);

        if (dir) {
            if (++pos == STRIP_LENGTH) {
                pos = 0;
            }
        } else {
            if (pos-- == 0) {
                pos = STRIP_LENGTH - 1;
            }
        }
    }
}

void bubble() {
    START_TIMER();

    while (true) {
        byte pixel = random(16);

        strip.setPixelColor(pixel, Wheel(random(256)));

        strip.show();

        delay(FRAME_LENGTH_MS);

        strip.setPixelColor(pixel, 0);

        CHECK_TIMER();
    }

}

void flash() {

    strip.setBrightness(255);

    START_TIMER();

    while (true) {
        uint32_t color = Wheel(random(256));

        for (byte q = 0; q < 16; q += 4) {
            strip.setPixelColor(q, color);
        }
        strip.show();

        delay(FRAME_LENGTH_MS);

        for (byte q = 0; q < 16; q += 4) {
            strip.setPixelColor(q, 0);
        }

        strip.show();

        delay(FRAME_LENGTH_MS * 15);

        CHECK_TIMER();
    }
}

void chasingRainbows() {
    START_TIMER();

    while (true) {
        for (byte pos = STRIP_LENGTH; pos > 0; pos--) {
            setPixel(pos, strip.Color(255, 0, 32));
            setPixel(pos + 1, strip.Color(160, 64, 0));
            setPixel(pos + 2, strip.Color(96, 96, 0));
            setPixel(pos + 3, strip.Color(0, 128, 0));
            setPixel(pos + 4, strip.Color(0, 0, 96));
            setPixel(pos + 5, strip.Color(32, 0, 64));
            setPixel(pos + 6, 0);

            strip.show();

            delay(FRAME_LENGTH_MS);

            CHECK_TIMER();
        }

    }
}

void chasingRainbowsReverse() {
    START_TIMER();

    while (true) {
        for (byte pos = 0; pos < STRIP_LENGTH; pos++) {
            setPixel(pos, strip.Color(255, 0, 0));
            setPixel(pos - 1, strip.Color(128, 96, 0));
            setPixel(pos - 2, strip.Color(96, 96, 0));
            setPixel(pos - 3, strip.Color(0, 128, 0));
            setPixel(pos - 4, strip.Color(0, 0, 96));
            setPixel(pos - 5, strip.Color(32, 0, 32));
            setPixel(pos - 6, 0);

            strip.show();

            delay(FRAME_LENGTH_MS);

            CHECK_TIMER();
        }

    }
}

void theaterChase(uint32_t c, byte factor) {

    factor = factor + 1; // factor 1: Every 4th pixel, factor 2: Every 8th pixel, etc.

    START_TIMER();

    while (true) {
        for (int q = 0; q < 4 * factor; q++) {
            for (int i = 0; i < STRIP_LENGTH; i = i + 4 * factor) {
                strip.setPixelColor(i + q, c);  //turn every fourth (or eighth) pixel on
            }
            strip.show();

            delay(FRAME_LENGTH_MS);

            for (int i = 0; i < STRIP_LENGTH; i = i + 4 * factor) {
                strip.setPixelColor(i + q, 0);      //turn every fourth (or eighth) pixel off
            }

            CHECK_TIMER();
        }
    }
}

void theaterChaseReverse(uint32_t c, byte factor) {

    factor = factor + 1; // factor 1: Every 4th pixel, factor 2: Every 8th pixel, etc.

    START_TIMER();

    while (true) {
        for (int q = 4 * factor; q > 0; q--) {
            for (int i = 0; i < STRIP_LENGTH; i = i + 4 * factor) {
                strip.setPixelColor(i + q - 1, c);  //turn every fourth (or eighth) pixel on
            }
            strip.show();

            delay(FRAME_LENGTH_MS);

            for (int i = 0; i < STRIP_LENGTH; i = i + 4 * factor) {
                strip.setPixelColor(i + q - 1, 0);      //turn every fourth (or eighth) pixel off
            }

            CHECK_TIMER();
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {

    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
