// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        2 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 1 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 100 // Time (in milliseconds) to pause between pixels
boolean blinking = false;

void setup() {
  Serial.begin(9600);
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  double Vout = analogRead(A2);
  double mappedVout = Vout *(3.9/1023.0);
  double ChargingVout = analogRead(A1);
  double mappedChargingVout = ChargingVout * (5/1023.0);
  //Serial.println(mappedChargingVout);
  //Serial.print("\n");
  Serial.println(mappedChargingVout);
  Serial.print("\n");

  if(mappedChargingVout > 0 && blinking){
     pixels.clear();
     pixels.show(); 
     blinking = false;
      delay(DELAYVAL);
  } else if(mappedChargingVout > 3.5 && !blinking) {
    blinking = true;
    delay(DELAYVAL);
  } else {
    blinking = false;
    delay(DELAYVAL);
  }
  
  if (mappedVout > 0.702){
    Serial.println("Voltage over high limit, something's wrong!");
    pixels.setPixelColor(0,pixels.Color(0, 0, 255));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }

  else if (mappedVout < 0.594){
    Serial.println("Voltage below low limit, something's wrong!");
    pixels.setPixelColor(0,pixels.Color(0, 0, 255));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
  
  else if (mappedVout >0.6 && mappedVout<0.702) {
    pixels.setPixelColor(0,pixels.Color(255, 0, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }

   else if (mappedVout>0.594 && mappedVout<0.6) {
     pixels.setPixelColor(0,pixels.Color(0, 255, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop

  }
  }
