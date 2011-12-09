//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//Network/OSC Enabled Strip Controller
//Needed 3rd Party Library:
//  -https://github.com/neophob/EthernetBonjour (Bonjour)
//  -https://github.com/neophob/WS2801-Library (WS2801)
//  -https://github.com/neophob/ArdOSC (AndOSC)

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
#define OSC_MSG_CHANGE_MODE_DIRECT "/modd"
#define OSC_MSG_AUDIO "/audio"

//*************************/
// WS2801
//how many pixels, I use 32 pixels/m
#define NR_OF_PIXELS 160
#define OSC_WORKARROUND_TIME 2

//output pixels dni:3/2
int dataPin = 2; 
int clockPin = 3;  

uint8_t DELAY = 20;
uint8_t delayTodo = 0;

//*************************/
// Network settings
byte myMac[] = { 0xAF, 0xFE, 0x00, 0xBE, 0x00, 0x01 };
byte myIp[]  = { 192, 168, 111, 222 };
//byte myIp[4]  = { 10, 0, 1, 111 };

int serverPort  = 10000;
byte oscCallBackWorkarround;

//*************************/
// Misc
#define MAX_NR_OF_MODES 6

WS2801 strip = WS2801(NR_OF_PIXELS, dataPin, clockPin);
int ledPin =  9;
uint8_t oscR, oscG, oscB, mode;
OSCServer oscServer;
int frames=0;

//use serail debug or not
#define USE_SERIAL_DEBUG 1

//uncomment it to enable audio
//#define USE_AUDIO_INPUT 1

//some common color defines
const uint32_t WHITE_COLOR = 0xffffff;

//*************************/
// Audio input
#ifdef USE_AUDIO_INPUT
  boolean isAudioVolumeEnabled;
  float audioVol;
  uint16_t maxVal;
#endif

/******************************************************************************************
 *  SETUP
 *****************************************************************************************/
void setup(){ 
 //init
 oscR = 255;
 oscG = 255;
 oscB = 255;
 mode=0;
 oscCallBackWorkarround = 0;

#ifdef USE_SERIAL_DEBUG
  Serial.begin(115200);
  Serial.println("INVDR!");
#endif

 Ethernet.begin(myMac ,myIp); 
 
 oscServer.begin(serverPort);
 
 //set callback function
 oscServer.addCallback(OSC_MSG_SET_R, &oscCallbackR); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_SET_G, &oscCallbackG); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_SET_B, &oscCallbackB); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_SET_DELAY, &oscCallbackDelay); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_CHANGE_MODE, &oscCallbackChangeMode); //PARAMETER: None, just a trigger
 oscServer.addCallback(OSC_MSG_CHANGE_MODE_DIRECT, &oscCallbackChangeModeDirect); //PARAMETER: None, just a trigger

#ifdef USE_AUDIO_INPUT
 oscServer.addCallback(OSC_MSG_AUDIO, &oscCallbackAudio); //PARAMETER: 1, int value 0..1
#endif
 
 //init effect
 setupLines(false);
 
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
  EthernetBonjour.begin("Invader");
  
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
  EthernetBonjour.addServiceRecord("Invader._osc",
                                   10000,
                                   MDNSServiceUDP);
  
}

/*****************************************************************************************
 *  LOOP
 *****************************************************************************************/  
void loop(){    
  // This actually runs the Bonjour module. YOU HAVE TO CALL THIS PERIODICALLY,
  // OR NOTHING WILL WORK! Preferably, call it once per loop().
  EthernetBonjour.run();
  
  if (oscServer.aviableCheck()>0){
    //we need to call available check to update the osc server
  }
  
  if (delayTodo>0) {
     //Delay not finished yet
     delayTodo--;
     delay(1);
  } else {
    //delay finished, update it
    delayTodo=DELAY;
    
#ifdef USE_AUDIO_INPUT
  if (frames%3==0) {
    loopAudioSensor();
  }
#endif
  
    switch (mode) {
      case 0:
          loopLines();
          break;
      case 1:  //SOLID Color White          
      case 2:  //SOLID Color Wheel      
          loopSolid();
          break;
      case 3:
          loopKnightRider();    
          break;
      case 4:
          loopRainbow();    
          break;
      case 5:
          loopLines();    
          break;
    }
    strip.show();    
    frames++;
  } 
  
  if (oscCallBackWorkarround>0) {
    oscCallBackWorkarround--;
  }
  
}

//just blink
void synchronousBlink() {
  digitalWrite(ledPin, HIGH);
  delay(20);
  digitalWrite(ledPin, LOW);  
}

void initMode() {
#ifdef USE_SERIAL_DEBUG
  Serial.print("SP ");
#endif  
  
  switch (mode) {
    case 0:
          setupLines(false);
          break;
    case 1:  
          setupSolid(0);
          break;
    case 2:
          setupSolid(1);
          break;
    case 3:
          setupKnightRider();    
          break;
    case 4:
          setupRainbow();    
          break;
    case 5:
          setupLines(true);    
          break;
  }  

  synchronousBlink();
  
#ifdef USE_SERIAL_DEBUG
  Serial.print("M:");
  Serial.println(mode);
#endif  
}

//*************************/
// OSC callback

// DELAY
void oscCallbackDelay(OSCMessage *_mes){
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;

  //delay between 0ms and 120ms
  DELAY = byte( _mes->getArgFloat(0)*120.0f );

#ifdef USE_SERIAL_DEBUG
  Serial.print("D:");
  Serial.println(DELAY, DEC);
#endif 
}

// R
void oscCallbackR(OSCMessage *_mes){
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;

  oscR = byte( _mes->getArgFloat(0)*255.f );

#ifdef USE_SERIAL_DEBUG
  Serial.print("R:");
  Serial.println(oscR, DEC);
#endif 
}

// G
void oscCallbackG(OSCMessage *_mes){
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;

  oscG = byte( _mes->getArgFloat(0)*255.f );

#ifdef USE_SERIAL_DEBUG
  Serial.print("G:");
  Serial.println(oscG, DEC);
#endif
}

// B
void oscCallbackB(OSCMessage *_mes){
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;

  oscB = byte( _mes->getArgFloat(0)*255.f );
  
#ifdef USE_SERIAL_DEBUG
  Serial.print("B:");
  Serial.println(oscB, DEC);
#endif  
}

#ifdef USE_AUDIO_INPUT  
// AUDIO
void oscCallbackAudio(OSCMessage *_mes){
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;

  int arg=_mes->getArgInt32(0);
  if (arg==1) {
    isAudioVolumeEnabled = true;
  } else {
    isAudioVolumeEnabled = false;
  }

#ifdef USE_SERIAL_DEBUG
  Serial.print("A:");
  Serial.println(isAudioVolumeEnabled, DEC);
#endif  
}
#endif

// change mode, use mode nr X
void oscCallbackChangeModeDirect(OSCMessage *_mes){
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;
  
  uint8_t arg=_mes->getArgInt32(0) & 0xff;
  if (arg > MAX_NR_OF_MODES-1) {
    return;
  }

  mode=arg;
  initMode(); 
}


// change mode, just increase current mode
void oscCallbackChangeMode(OSCMessage *_mes){
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;

  uint8_t arg=_mes->getArgInt32(0) & 0xff;
  if (arg == 0) {
    return;
  }
  
  if (mode<MAX_NR_OF_MODES-1) {
    mode++;
  } else {
    mode = 0; 
  }

  initMode(); 
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


//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170; 
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

