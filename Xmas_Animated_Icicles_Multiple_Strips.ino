/*
 * Code to similuate dripping icicles using Arduino and multiple NeoPixel strips
 * be sure to weatherize if you are putting this outdoors
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN1 3
#define PIN2 4
#define PIN3 5
#define PIN4 6
#define PIN5 7

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);

const int numberOfStrips = 5;

// initialize strips using an array
// if you use a number of strips differing from 5, then the array needs to be adjusted accordingly
Adafruit_NeoPixel strip[numberOfStrips] = 
{
  Adafruit_NeoPixel(8, PIN1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(8, PIN2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(8, PIN3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(8, PIN4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(8, PIN5, NEO_GRB + NEO_KHZ800)
  };


// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.



void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  Serial.begin(9600);

  for(int i = 0; i < numberOfStrips; i++){
    strip[i].begin();
    ledsOff(i, 1);   // Initialize all pixels to 'off'
    strip[i].show(); 
  };

  randomSeed(analogRead(A0));  // make sure nothing is connected to A0 so that random noise acts as seed

}


// using a photoresistor in series with a 1 kOhm resistor to turn on strips in darkness only
int darknessThreshold = 220;  //configure to your preferences 


void loop() 
{
  int lightValue = analogRead(A0);
  int dripRate = 150;               // time in mSec each pixel is lit

  if( lightValue < darknessThreshold )
  {
    // let's randomize which strip gets lit and the delay between strips being lit
    int randomStrip = random(0, numberOfStrips);
    int randomDelay = random(0, 2000);

    drippingIcicle(randomStrip, dripRate);

    ledsOff(randomStrip, 500);

    delay(randomDelay);


    /* //cycle through each strip in order - boring
    for(int i = 0; i <= numberOfStrips; i++)
    {
       drippingIcicle(i, dripRate);
       ledsOff(i, 500);
    }
    */
  }
}



//Dripping icicle by @GreenMoonArt
void drippingIcicle(int i, uint8_t wait) 
{
  // this code is set up for 5 strips with 8 neopixels per strip
  // colors are set up to have brightest at the top (color1)
  // currenly 4 colors move down the strip as strip is hung vertically with DataIn at the top
  // to do: set up math to accommate different numbers of strips and neopixels
  
  uint32_t color1 = strip[i].Color(91, 204, 222);
  uint32_t color2 = strip[i].Color(4, 16, 18);
  uint32_t color3 = strip[i].Color(8, 8, 8);
  uint32_t color4 = strip[i].Color(1, 1, 1);
  uint32_t colorOff = strip[i].Color(0, 0, 0);

  for (int j=0; j < strip[i].numPixels()+3; j++) 
  {  

    if(j >= 3)   strip[i].setPixelColor(j-3, color1);

    if(j >= 2) strip[i].setPixelColor(j-2, color2);

    if(j >= 1)  strip[i].setPixelColor(j-1, color3);

    strip[i].setPixelColor(j, color4);

    // turn off trailing LEDs
    if( j >= 4)
      {
      strip[i].setPixelColor(j-4, colorOff);
      }

    strip[i].show();

    delay(wait);

  }

}


void ledsOff(int i, uint8_t wait)
{
  for(uint16_t j=0; j<strip[i].numPixels(); j++) 
    {
    strip[i].setPixelColor(j, strip[i].Color(0, 0, 0));
    }

  strip[i].show();
  delay(wait);
}





