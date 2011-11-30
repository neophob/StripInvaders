//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//

#include <SPI.h>
#include <Ethernet.h>
#include <ArdOSC.h>
#include "WS2801.h"

//*************************/
// OSC Stuff
#define OSC_MSG_SET_R "/stripinv/r"
#define OSC_MSG_SET_G "/stripinv/g"
#define OSC_MSG_SET_B "/stripinv/b"

#define OSC_MSG_CHANGE_MODE "/stripinv/mode"

//*************************/
// WS2801
//how many pixels
#define NR_OF_PIXELS 64
//output pixels
int dataPin = 2;       
int clockPin = 3;  

static uint8_t DELAY = 34;

//*************************/
// Network settings
byte myMac[] = { 0xAF, 0xFE, 0x00, 0xBE, 0x00, 0x01 };
byte myIp[]  = { 192, 168, 0, 177 };
int  serverPort  = 10000;

//*************************/
// Misc
#define MAX_NR_OF_MODES 2

WS2801 strip = WS2801(NR_OF_PIXELS, dataPin, clockPin);
int ledPin =  9;
uint8_t oscR, oscG, oscB, mode;
OSCServer server;

/**
 *  SETUP
 */
void setup(){ 
 mode=0;
 Serial.begin(19200);
 
 Ethernet.begin(myMac ,myIp); 
 server.begin(serverPort);
 
 //set callback function
 server.addCallback(OSC_MSG_SET_R, &oscCallbackR);
 server.addCallback(OSC_MSG_SET_G, &oscCallbackG);
 server.addCallback(OSC_MSG_SET_B, &oscCallbackB);
 server.addCallback(OSC_MSG_CHANGE_MODE, &oscCallbackChangeMode); 

 //init
 oscR = 255;
 oscG = 255;
 oscB = 255;
 
  //ws2801 start strips 
 strip.begin();

 setupLines();
 pinMode(ledPin, OUTPUT);  
}

/**
 *  LOOP
 */  
void loop(){
  switch (mode) {
    case 0:
          loopLines();
          break;
    case 1:
          loopStars();    
          break;
  }  
//  digitalWrite(ledPin, HIGH);
//  delay(50);
//  digitalWrite(ledPin, LOW);
}

//*************************/
// OSC callback
void oscCallbackR(OSCMessage *_mes){
  //get 1st argument(int32)
  oscR = _mes->getArgInt32(0) & 255;
}

void oscCallbackG(OSCMessage *_mes){
  //get 1st argument(int32)
  oscG = _mes->getArgInt32(0) & 255;
}

void oscCallbackB(OSCMessage *_mes){
  //get 1st argument(int32)
  oscB = _mes->getArgInt32(0) & 255;
}

void oscCallbackChangeMode(OSCMessage *_mes){
  if (mode<MAX_NR_OF_MODES) {
    mode++;
  } else {
    mode = 0; 
  }
  
  switch (mode) {
    case 0:
          setupLines();
          break;
    case 1:
          setupStars();    
          break;
  }  
}

//*************************/
// Helper Functions

void setTintPixelColor(uint16_t i, uint32_t c) {
  uint16_t b = c & 0xff;
  c >>= 8;
  uint16_t g = c & 0xff;
  c >>= 8;
  uint16_t r = c & 0xff;

  if (oscR = 255 && oscG == 255 && oscB == 255) {
    //no tint effect, no calculations needed
  } else {
    //apply tint effect
    r = r*oscR >> 8;
    g = g*oscG >> 8;
    b = b*oscB >> 8;
  }
  
  uint32_t tintCol = r & 255;
  tintCol <<= 8;
  tintCol |= g & 255;
  tintCol <<= 8;
  tintCol |= b & 255;
  strip.setPixelColor(i, tintCol);
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
