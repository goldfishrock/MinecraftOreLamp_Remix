# MinecraftOreLamp_Remix
Remixed code for  Danjhamer's amazing Minecraft Ore Lamp (https://www.thingiverse.com/thing:3401758)

I decided to make this for my lad who is a recent convert and is now totally obsessed with everything Minecraft. There are plenty 'Ore Block' projects on Thingiverse but this one looked far better than the others. I liked the attention to detail in the model, the base had been well designed, pegs to hold the choc-block connector, a tactile switch and the Arduino. Everything fitted perfectly. The post for the LEDS was millimetre perfect so you don’t have to cut the strip to position the first four pixels, one of the best thought out designs I’d seen.

After wiring it all up and blowing the code to the Arduino I found that I was getting five entirely different colours but when testing it with the example Adafruit Neopixel Sketches included with the library it was working fine, so I knew the hardware and wiring was correct. After a little digging I found that there were several choices when the Adafruit_NeoPixel object was created

_ _ NEO_KHZ800 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
NEO_KHZ400 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
NEO_GRB Pixels are wired for GRB bitstream (most NeoPixel products)
NEO_RGB Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
NEO_RGBW Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
(Source: https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use) _ _


I changed mine to the GRB version and it worked straight away. The updated code can be found here: https://github.com/goldfishrock/MinecraftOreLamp_Remix

I wasn’t familiar with the Neopixel library or how to control individually addressable LEDs and I couldn’t find any info on the wiring on the original Thingiverse  post. So just in case anyone else is a little confused, this is how I connected mine.


**Arduino Pin**
- 5		->		Button Pin
- 7		->		DIN (Data In on LED strip)
- GND		->		Common Ground (choc-bloc)
- RAW/VCC	->		Common 5V (choc-bloc)

**LED Strip**
- +5v		->		Common 5V (choc-bloc)
- DIN		->		Pin 7 (Arduino)
- GND		-> 		Common Ground (choc-bloc)

**Button**
- Pin 1		->		Pin 5 (Arduino)
- Pin 2		->		Common Ground (choc-bloc)

A 5v supply is then connected to the other side of the two ‘commons’ choc-bloc.

