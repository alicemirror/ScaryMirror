/**
 * @file  ScaryMirror_NeoPixel
 * @brief Neopixel functions on the Arduino. This version if for neopixel testing
 * launching the lighting functions in sequence.
 * 
 * Incudes the time one interrupt to manage a 5 ms pulse every 5 sec
 * to keep alive the smartphone battery pack (Arduino board and neopixel strip)
 * 
 * @author Enrico Miglino <balearicdynamics@gmail.com>
 * @date Sep.-Oct. 2018
 * 
 */

#include <Adafruit_NeoPixel.h>
#include <TimerOne.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NEOPIXEL_PIN 6

//! Pulse pin to avoid the power pack automatically shut down.
#define PULSE_PIN 5
//! Uses the builtin LED to monitor the pulse generation to keep the power bank
//! battery always on
#define PULSE_MONITOR LED_BUILTIN
//! Pulse duration. Should be empirically calibrated on the power bank used
#define PULSE_MS 5
//! Interval between two pulses (long interval, about 1 second)
#define PULSE_INTERVAL 5000000
//! Number of Neopixel LEDs in the strip
#define NEOPIXEL_LEDS 144

/**
    @note Note on Neopixel settings \n
    Parameter 1 = number of pixels in strip \n
    Parameter 2 = Arduino pin number (most are valid) \n
    Parameter 3 = pixel type flags, add together as needed: \n
    NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs) \n
    NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers) \n
    NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products) \n
    NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2) \n
    NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
    
    @warning To reduce NeoPixel burnout risk, add 1000 uF capacitor across
    pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
    and minimize distance between Arduino and first pixel.  Avoid connecting
    on a live circuit...if you must, connect GND first.
 */
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_LEDS , NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

//! Startup and initialization
void setup() {

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  pinMode(PULSE_PIN, OUTPUT);
  pinMode(PULSE_MONITOR, OUTPUT);

  digitalWrite(PULSE_PIN, HIGH);
  digitalWrite(PULSE_MONITOR, LOW);

  Timer1.initialize(PULSE_INTERVAL);
  Timer1.attachInterrupt(pulsePower);
  Timer1.start();
}

// Predefined colors
#define PINK strip.Color(255, 64, 64)
#define WHITE strip.Color(255, 255, 255)

#define FIRE1 strip.Color(255, 64, 0)
#define FIRE2 strip.Color(255, 96, 0)
#define FIRE3 strip.Color(255, 128, 0)
#define FIRE4 strip.Color(255, 96, 32)
#define FIRE5 strip.Color(255, 32, 64)
#define FIRE6 strip.Color(255, 32, 96)

#define BLUE1 strip.Color(0, 0, 255)
#define BLUE2 strip.Color(0, 32, 255)
#define BLUE3 strip.Color(0, 64, 255)
#define BLUE4 strip.Color(0, 96, 255)

#define PURPLE1 strip.Color(96, 16, 255)
#define PURPLE2 strip.Color(96, 16, 128)
#define PURPLE3 strip.Color(96, 16, 96)
#define PURPLE4 strip.Color(96, 16, 64)

//! Main loop
void loop() {

    // ==========================================
    // TEST COMMANDS
    // ==========================================
    
    cmdFlash();
    delay(2500);

    // ==========================================
    // TEST FUNCTIONS
    // ==========================================

    // Rotate the strip with one color on and off 
    colorWipeOnOff(FIRE1, 10);
    colorWipeOnOff(FIRE2, 10);
    colorWipeOnOff(FIRE3, 10);

    // Rotate the strip with one color
    colorWipe(FIRE1, 10);
    colorWipe(FIRE2, 10);
    colorWipe(FIRE3, 10);
    colorWipe(FIRE4, 10);
    colorWipe(FIRE5, 10);
    colorWipe(FIRE6, 10);

    // Flash colors
    colorFlash(FIRE1, 10);
    colorFlash(FIRE2, 10);
    colorFlash(FIRE3, 10);
    colorFlash(FIRE4, 10);
    colorFlash(FIRE5, 10);
    colorFlash(FIRE6, 10);

    // Fixed colors
    setColor(FIRE1);
    delay(1000);
    setColor(FIRE2);
    delay(1000);
    setColor(FIRE3);
    delay(1000);
    setColor(FIRE4);
    delay(1000);
    setColor(FIRE5);
    delay(1000);
    setColor(FIRE6);
    delay(1000);

    setColor(BLUE1);
    delay(500);
    setColor(BLUE2);
    delay(500);
    setColor(BLUE3);
    delay(500);
    setColor(BLUE4);
    delay(500);
    
    setColor(PURPLE1);
    delay(500);
    setColor(PURPLE2);
    delay(500);
    setColor(PURPLE1);
    delay(500);
    setColor(PURPLE4);
    delay(500);

//  rainbow(10);
//  rainbowCycle(10);

}

/**
 * Executes the specific command flashing the white light for
 * a predefined time
 */
void cmdFlash() {
    colorFlash(BLUE1, 1000);
}

//! Timer callback function.
//! Stop the timer, output the pin for 20 ms then restart the timer
void pulsePower(void)
{
  Timer1.stop();
  digitalWrite(PULSE_PIN, LOW);  
  digitalWrite(PULSE_MONITOR, LOW);
  delay(PULSE_MS);
  digitalWrite(PULSE_PIN, HIGH);
  digitalWrite(PULSE_MONITOR, HIGH);
  Timer1.start();
}

//! Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

//! Fill the dots one after the other with a color
//! Then off in the same sequence
void colorWipeOnOff(uint32_t c, uint8_t wait) {
  // Pixels on
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
  // Pixels off
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0x00);
    strip.show();
    delay(wait);
  }
}

void colorFlash(uint32_t c, uint8_t wait) {
    setColor(c);
    delay(wait);
    setColor(0);
}

void setColor(uint32_t c) {
    uint16_t i;
    
    for(i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
    }
    strip.show();
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
