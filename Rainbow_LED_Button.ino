/*
    Arcade Rainbow LED Button using Arduino Pro Mini 3.3V for RetroPie
    Author       : zzeromin, member of Tentacle Team and Raspberrypi Village
    Creation Date: Mar 8, 2017
    Cafe         : http://cafe.naver.com/raspigamer
    Blog         : http://rasplay.org, http://forums.rasplay.org/, https://zzeromin.tumblr.com/
    Thanks to    : 9p, angel, csDevil, greatKStar, kimPanda, smyani
    Free and open for all to use. But put credit where credit is due.
    Reference 1: Neopixel Library for Arduino
    https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library
    https://learn.adafruit.com/neopixel-arcade-button/assembly?view=all
    Reference 2: Arduino - Dual Function Button - Long Press/Short Press (Without Delay) + Bonus
    https://www.instructables.com/id/Arduino-Dual-Function-Button-Long-PressShort-Press/
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 2
#define NUMPIXELS 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// example RGB Color Codes
uint32_t red = strip.Color(255, 0, 0);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t skyblue = strip.Color(135, 206, 235);
uint32_t aqua = strip.Color(0, 255, 255);
uint32_t magenta = strip.Color(255, 0, 255);
uint32_t pink = strip.Color(255, 0, 127);
uint32_t white = strip.Color(255, 255, 255);

int delayBlink = 20; // delay for blink
int buttonState[6] = {}; // state for six buttons

long buttonTimer = 0;
long longPressTime = 500;

boolean buttonActive = false;
boolean longPressActive = false;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  for (int i = 7; i < 13; i++)
    pinMode(i, INPUT);
  //Serial.begin(9600);
  setupLed(); // Initialize all pixels to 'on'
}

void loop() {
  RainbowLedButton(12, 0, yellow);
  LedButtonOnOff(digitalRead(11), 1, skyblue);
  LedButtonOnOff(digitalRead(10), 2, pink);
  LedButtonOnOff(digitalRead(9), 3, aqua);
  LedButtonOnOff(digitalRead(8), 4, magenta);
  LedButtonOnOff(digitalRead(7), 5, red);
}

// Initialize all pixels to 'on' and red color
void setupLed(void)
{
  uint16_t i;
  for ( i = 0; i < 6; i++ )
    strip.setPixelColor( i, red ); // you can select color whatever you want.
  strip.show(); // This sends the updated pixel color to the hardware.
  delay(20); // Delay for a period of time (in milliseconds)
}

// Led off when you push button
void LedButtonOnOff(int buttonState, int buttonNum, uint32_t color)
{
  if (buttonState == HIGH) {
    strip.setPixelColor( buttonNum, color ); // Moderately bright RED color.
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(delayBlink); // Delay for a period of time (in milliseconds)
  }
  else {
    strip.setPixelColor( buttonNum, 0, 0, 0 ); // no color
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(delayBlink); // Delay for a period of time (in milliseconds)
  }
}

// Led colors look like rainbow and fast blink when you push button long time
void RainbowLedButton(int buttonPin, int buttonNum, uint32_t color)
{
  if ( digitalRead(buttonPin) == LOW ) {

    if ( buttonActive == false ) {
      buttonActive = true;
      buttonTimer = millis();
      //Serial.println(buttonTimer);
      LedButtonOnOff(LOW, buttonNum, color);
    }

    if ( (millis() - buttonTimer > longPressTime) && (longPressActive == false) ) {
      longPressActive = true;
      rainbowCycle(1);
    }

  } else {

    if ( buttonActive == true ) {

      if ( longPressActive == true ) {
        longPressActive = false;
        theaterChase(strip.Color(255, 0, 0), 15); // Red
      } else {
      }

      buttonActive = false;
      LedButtonOnOff(HIGH, buttonNum, color);

    }
  }
}

// Next functions made by Adafruit.
// Reference: https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 4; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
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
