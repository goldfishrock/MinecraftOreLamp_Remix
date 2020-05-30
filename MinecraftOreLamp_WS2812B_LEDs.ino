   /*****************************************************************
   *                      Minecraft Ore Lamp                        *
   *                           V0.1                                 *
   *                                                                *
   *                      By Dan J Hamer                            *
   *                          2018                                  *
   *                   http://carbonfrog.com                        *
   *                                                                *
   *        This work is licensed under a Creative Commons          *
   *        Attribution-ShareAlike 4.0 International License.       *
   *        http://creativecommons.org/licenses/by-sa/4.0/          *
   *                                                                *
   * v1.0 - Original                                                *                
   * v1.1 - Changed NEOPIXEL type from NEO_GRBW to NEO_GRB for      *
   *        WS2812B LED Strip                                       *
   *        (https://www.amazon.co.uk/gp/product/B07TJDS1PZ)        *
   *                                                                *
   *****************************************************************/
#include <Adafruit_NeoPixel.h>
//#include <EEPROM.h>

#define LEDPIN 5
#define PIXELS 5
#define BUTTON1 7

boolean  dimDir = 1;
byte brightness = 127;
unsigned long currentMillis = 0;
byte colour = 1;
byte eventOutput = 2;
byte eventOutputLast = 0;
byte brightnessSetting;


#define DEBOUNCE 20
#define DOUBLECLICKPERIOD 500
#define HOLDCLICKPERIOD 1000

// 32bit colour protocol 0xWWRRGGBB
//#define COAL        0x00000000
//#define DIAMOND     0x0000FFFF
//#define LAPIS       0x004169E1
//#define EMERALD     0x0035C862
//#define REDSTONE    0x00FF0000
//#define GOLD        0x00FFD700
//#define IRON        0x00E4C697
//#define SILVER      0xDCDCDCDC

#define COAL        0x00221F1F
#define DIAMOND     0xFF00FFFF
#define LAPIS       0x000000FF
#define EMERALD     0x0000FF00
#define REDSTONE    0x00FF0000
#define GOLD        0x00FFD700
#define IRON        0x00B28E2A
#define SILVER      0xDCDCDCDC


uint32_t colourPalette [8] =
{
  COAL,
  DIAMOND,
  LAPIS,
  EMERALD,
  REDSTONE,
  GOLD,
  IRON,
  SILVER
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(brightness);
  
  strip.show(); // Initialize all pixels to 'off'
    
  pinMode(BUTTON1, INPUT_PULLUP);
}


void loop() {

  if ( eventOutput != eventOutputLast)
  {
    eventOutputLast = eventOutput;
  }

switch (eventOutput)
  {
    case 0:
      break;

    case 1:
        colour++;
        if ( colour >= 8 )
        {
          colour = 1;
        }
        lampColourChange(brightness);
      break;

    case 2:
      lampOff();
      break;

    case 3:
      lampBrightnessChange();
      break;

    default:
      break;   
  }
  eventOutput = buttonScan();
}

void lampBrightnessChange()
{
    if( dimDir && brightness < 255 )
    {
      brightness++;
    }
    else if( !dimDir && brightness > 0)
    {
      brightness--;
    }
    lampColourChange(brightness);
    delay(20);
    return;
}


void lampColourChange(byte bright)
{
//  Serial.println("lampColourChange");
    for(byte n=0; n < 5; n++)
    {
      strip.setPixelColor(n, colourPalette[colour]);
    }
    strip.setBrightness(bright);
    strip.show(); 
     
    return;
}


byte buttonScan()
{
  static unsigned long eventStartTime = 0;
  static byte eventStage = 0;
  static byte clickType = 0;      // 0 no event, 1 single click, 2 double click, 3 hold click,
  static unsigned long debounceEnd = 0;
  unsigned long eventCurrentTime = millis();
  
  switch (eventStage)
  {
    case 0:
      //start looking for button down
      if ( !digitalRead(BUTTON1)) // BUTTON1 depressed
      {
        eventStartTime = eventCurrentTime;
        eventStage = 1;
        debounceEnd = eventCurrentTime + DEBOUNCE;     
      }
      break;

    case 1:
      //debounce
      if ( debounceEnd < eventCurrentTime ) // BUTTON1 depressed
      {
        eventStage = 2;     
      }
      break;

    case 2:
      //looking for release before hold click time
      if ( digitalRead(BUTTON1) && eventCurrentTime < (eventStartTime+HOLDCLICKPERIOD) ) // BUTTON1 released before hold click time 2000mS
      {
        eventStage = 3; //goto debounce after button release
        debounceEnd = eventCurrentTime + DEBOUNCE;     
      }
      if ( !digitalRead(BUTTON1) && eventCurrentTime > (eventStartTime+HOLDCLICKPERIOD) )
      {
        eventStage = 10; //goto click hold stage.
      }
      break;

    case 3:
      //1st release Debounce
      if ( debounceEnd < eventCurrentTime ) // BUTTON1 released
      {
        eventStage = 4;     
      }
      break;

    case 4:
      //looking for 2nd depress before end of single click time or no click for single click
      if ( !digitalRead(BUTTON1) && eventCurrentTime < (eventStartTime+DOUBLECLICKPERIOD) ) // BUTTON1 released before end of double click time 1000mS
      {
        eventStage = 5; //goto 2nd click down debounce
        debounceEnd = eventCurrentTime + DEBOUNCE;     
      }
       // if no 2nd click goto single click terminator.
      else if ( digitalRead(BUTTON1) && eventCurrentTime > (eventStartTime+DOUBLECLICKPERIOD) )
      {
        eventStage = 8; //goto single click terminator.
      }     
      break;

    case 5:
      // 2nd Debounce 
      if ( debounceEnd < eventCurrentTime ) // BUTTON1 depressed
      {
        eventStage = 6;     
      }
      break;     
      
    case 6:
      //looking for release of 2nd click for double click 
      if ( digitalRead(BUTTON1) ) // BUTTON1 released before hold click time 2000mS
      {
        eventStage = 7; //goto debounce after 2nd button release
        debounceEnd = eventCurrentTime + DEBOUNCE;     
      }
      break;
      
    case 7:
      // 2nd release Debounce 
      if ( debounceEnd < eventCurrentTime ) // BUTTON1 depressed
      {
        eventStage = 9;     
      }
      break;

    case 8:
      //single click terminator
      clickType = 1; // Single click event
      eventStage = 12; // reset
      break;
      
    case 9:
      //double click terminator
      clickType = 2; // Double click event
      eventStage = 12; // reset
      break;

    case 10:
      //hold click
      if ( !digitalRead(BUTTON1) ) // BUTTON1 still depressed
      {
        clickType = 3; // hold click   
      }
      else if ( digitalRead(BUTTON1) )
      {
        eventStage = 11; //goto click hold release debounce.
        debounceEnd = eventCurrentTime + DEBOUNCE; 
      }
      break;    
    
    case 11:
      // click hold release Debounce
      if ( debounceEnd < eventCurrentTime ) // BUTTON1 depressed
      {
        eventStage = 12; // end click hold, reset
        dimDir = !dimDir;
      }
      break;    
 
    case 12:
      //reset button scan
      clickType = 0; // no event
      eventStage = 0; // reset
      break;
       
    default:
      //default
      clickType = 0; // no event
      eventStage = 0; // reset
      break;   
  }
 return clickType; 
}

void lampOff()
{
byte dimLevel = brightness;
  for( byte dim = dimLevel; dim > 0; dim--)
  {
    dimLevel = dim-1;
    delay(10);
    strip.setBrightness(dimLevel);
    strip.show();
  }
  //colour = 0;
 if (brightness < 10)
 {
  brightness = 127;  
 }
  while( !buttonScan() )
  {
    //In standby/off mode
  }
  
//  turningOff = 0;
  turnOn();
  return;
}

void turnOn()
{
byte dimLevel2 = 0;

  for( byte dim = 0; dim < brightness; dim++)
  {
    dimLevel2 = dim;
    delay(10);
    lampColourChange(dimLevel2);
  }
  eventOutput = 0;
  return;
}
