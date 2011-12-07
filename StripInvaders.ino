//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//Network/OSC Enabled Strip Controller
//Needed 3rd Party Library:
//  -http://gkaindl.com/software/arduino-ethernet/bonjour (Bonjour)
//  -https://github.com/neophob/WS2801-Library (WS2801)

#include <SPI.h>
#include <Ethernet.h>
#include <ArdOSC.h>
#include "WS2801.h"
#include <EthernetBonjour.h>

//*************************/
// OSC Stuff
#define OSC_MSG_SET_R "/knbb" //simple method to fix the fancy strip color order
#define OSC_MSG_SET_G "/knbg"
#define OSC_MSG_SET_B "/knbr"
#define OSC_MSG_SET_DELAY "/delay"
#define OSC_MSG_CHANGE_MODE "/mode"
#define OSC_MSG_AUDIO "/audio"

#define OSC_WORKARROUND_TIME 2
//*************************/
// WS2801
//how many pixels
#define NR_OF_PIXELS 160

//output pixels
int dataPin = 3;       
int clockPin = 2;  

uint8_t DELAY = 20;


//*************************/
// Audio input
boolean isAudioVolumeEnabled;
float audioVol;
uint16_t maxVal;


//*************************/
// Network settings
byte myMac[6] = { 0xAF, 0xFE, 0x00, 0xBE, 0x00, 0x01 };
byte myIp[4]  = { 192, 168, 111, 222 };
int serverPort  = 10000;
int oscCallBackWorkarround;


//*************************/
// Misc
#define MAX_NR_OF_MODES 5

WS2801 strip = WS2801(NR_OF_PIXELS, dataPin, clockPin);
int ledPin =  9;
uint8_t oscR, oscG, oscB, mode;
OSCServer oscServer;

#define USE_SERIAL_DEBUG 1

//uncomment it to enable audio
//#define USE_AUDIO_INPUT 1

/**
 *  SETUP
 */
void setup(){ 
 Ethernet.begin(myMac ,myIp); 
 oscServer.begin(serverPort);
 
 //set callback function
 oscServer.addCallback(OSC_MSG_SET_R, &oscCallbackR); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_SET_G, &oscCallbackG); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_SET_B, &oscCallbackB); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_SET_DELAY, &oscCallbackDelay); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_CHANGE_MODE, &oscCallbackChangeMode); //PARAMETER: None, just a trigger
 oscServer.addCallback(OSC_MSG_AUDIO, &oscCallbackAudio); //PARAMETER: 1, int value 0..1

 //init
 oscR = 255;
 oscG = 255;
 oscB = 255;
 mode=0;
 oscCallBackWorkarround = 0;
 
 setupLines();
 pinMode(ledPin, OUTPUT);  
 
 //we-are-ready indicator
 synchronousBlink();
 delay(50);
 synchronousBlink();
 delay(50);
 synchronousBlink();

 //ws2801 start strips 
 strip.begin();
 
  // Initialize the Bonjour/MDNS library. You can now reach or ping this
  // Arduino via the host name "arduino.local", provided that your operating
  // system is Bonjour-enabled (such as MacOS X).
  // Always call this before any other method!
  EthernetBonjour.begin("StripController");
  
  // Now let's register the service we're offering (a web service) via Bonjour!
  // To do so, we call the addServiceRecord() method. The first argument is the
  // name of our service instance and its type, separated by a dot. In this
  // case, the service type is _http. There are many other service types, use
  // google to look up some common ones, but you can also invent your own
  // service type, like _mycoolservice - As long as your clients know what to
  // look for, you're good to go.
  // The second argument is the port on which the service is running. This is
  // port 80 here, the standard HTTP port.
  // The last argument is the protocol type of the service, either TCP or UDP.
  // Of course, our service is a TCP service.
  // With the service registered, it will show up in a Bonjour-enabled web
  // browser. As an example, if you are using Apple's Safari, you will now see
  // the service under Bookmarks -> Bonjour (Provided that you have enabled
  // Bonjour in the "Bookmarks" preferences in Safari).
  EthernetBonjour.addServiceRecord("StripController._http",
                                   10000,
                                   MDNSServiceTCP);
#ifdef USE_SERIAL_DEBUG
  Serial.begin(115200);
  Serial.println("Hello World!");
#endif
}

/**
 *  LOOP
 */  
void loop(){    
  // This actually runs the Bonjour module. YOU HAVE TO CALL THIS PERIODICALLY,
  // OR NOTHING WILL WORK! Preferably, call it once per loop().
  EthernetBonjour.run();
  
  if (oscServer.aviableCheck()>0){
     //Serial.println("alive! "); 
  }
  
  if (oscCallBackWorkarround>0) {
    oscCallBackWorkarround--;
  }
  
#ifdef USE_AUDIO_INPUT
  loopAudioSensor();
#endif
  
  switch (mode) {
    case 0:
          loopLines();
          break;
    case 1:
          loopStars();    
          break;
    case 2:
          loopSolid();    
          break;
    case 3:
          loopKnightRider();    
          break;
    case 4:
          loopRainbow();    
          break;
  }
  
}

//convert a float value to a byte value
uint8_t getRgbValueFromFloat(float f) {
  f *= 255.0f;
  return byte(f);
}

//just blink
void synchronousBlink() {
  digitalWrite(ledPin, HIGH);
  delay(20);
  digitalWrite(ledPin, LOW);  
}

//*************************/
// OSC callback

void oscCallbackDelay(OSCMessage *_mes){
  //workarround that osc messages arrives twice...
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;
  
  DELAY = byte( _mes->getArgFloat(0)*100.0f );
  synchronousBlink();

#ifdef USE_SERIAL_DEBUG
  Serial.print("Delay: ");
  Serial.println(DELAY, DEC);
#endif 
}

// R
void oscCallbackR(OSCMessage *_mes){
  //workarround that osc messages arrives twice...
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;
  
  oscR = getRgbValueFromFloat( _mes->getArgFloat(0) );
  synchronousBlink();

#ifdef USE_SERIAL_DEBUG
  Serial.print("R: ");
  Serial.println(oscR, DEC);
#endif 
}

// G
void oscCallbackG(OSCMessage *_mes){
  //workarround that osc messages arrives twice...
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME; 
  
  oscG = getRgbValueFromFloat( _mes->getArgFloat(0) );
  synchronousBlink();  

#ifdef USE_SERIAL_DEBUG
  Serial.print("G: ");
  Serial.println(oscG, DEC);
#endif
}

// B
void oscCallbackB(OSCMessage *_mes){
  //workarround that osc messages arrives twice...
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;

  oscB = getRgbValueFromFloat( _mes->getArgFloat(0) );
  synchronousBlink();  
  
#ifdef USE_SERIAL_DEBUG
  Serial.print("B: ");
  Serial.println(oscB, DEC);
#endif  
}


void oscCallbackAudio(OSCMessage *_mes){
  int arg=_mes->getArgInt32(0);
  if (arg==1) {
    isAudioVolumeEnabled = true;
  } else {
    isAudioVolumeEnabled = false;
  }
  
}


// change mode
void oscCallbackChangeMode(OSCMessage *_mes){
  //workarround that osc messages arrives twice...
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;
  
  int arg=_mes->getArgInt32(0);
  if (arg != 1) {
    return;
  }
  
  if (mode<MAX_NR_OF_MODES-1) {
    mode++;
  } else {
    mode = 0; 
  }

#ifdef USE_SERIAL_DEBUG
  Serial.print("Setup start: ");
#endif  
  
  switch (mode) {
    case 0:
          setupLines();
          break;
    case 1:
          setupStars();    
          break;
    case 2:
          setupSolid();    
          break;
    case 3:
          setupKnightRider();    
          break;
    case 4:
          setupRainbow();    
          break;
  }  

  synchronousBlink();
  
#ifdef USE_SERIAL_DEBUG
  Serial.print("mode: ");
  Serial.println(mode);
#endif  
  
}

//*************************/
// Helper Functions

//TODO: verify parameter Passing by value
void setTintPixelColor(uint16_t i, uint32_t c) {
  uint16_t b = c & 0xff;
  c >>= 8;
  uint16_t g = c & 0xff;
  c >>= 8;
  uint16_t r = c & 0xff;

  if (oscR == 255 && oscG == 255 && oscB == 255) {
    //no tint effect, no calculations needed
  } else {
    //apply tint effect and SWAP color according to real cabeling
    r = r*(oscR+1) >> 8;
    g = g*(oscG+1) >> 8;
    b = b*(oscB+1) >> 8;
  }
  
#ifdef USE_AUDIO_INPUT  
  //audio volume fx
  if (isAudioVolumeEnabled) {
    r *= audioVol;
    g *= audioVol;
    b *= audioVol;
  }
#endif
  
  strip.setPixelColor(i, r, g, b);
}


// Create a 24 bit color value from R,G,B
uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
