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
#include <EEPROM.h>

//*************************/
// OSC Stuff
#define OSC_MSG_SET_R "/knbb" //simple method to fix the fancy strip color order
#define OSC_MSG_SET_G "/knbg"
#define OSC_MSG_SET_B "/knbr"
#define OSC_MSG_SET_DELAY "/delay"
#define OSC_MSG_CHANGE_MODE "/mode"
#define OSC_MSG_CHANGE_MODE_DIRECT "/modd"
#define OSC_MSG_AUDIO "/audio"
#define OSC_MSG_SWAP_CABELING "/swp"

//*************************/
// WS2801
//how many pixels, I use 32 pixels/m
#define NR_OF_PIXELS 160
#define OSC_WORKARROUND_TIME 2

//output pixels dni:3/2
int dataPin = 2; 
int clockPin = 3;  

//dummy init the ws2801 lib
WS2801 strip = WS2801(NR_OF_PIXELS, dataPin, clockPin); 

//*************************/
// Network settings
byte myMac[] = { 0xAF, 0xFE, 0x00, 0xBE, 0x00, 0x01 };
byte myIp[]  = { 192, 168, 111, 222 };
//byte myIp[4]  = { 10, 0, 1, 111 };

int serverPort  = 10000;
byte oscCallBackWorkarround;
OSCServer oscServer;

//*************************/
// Fader

const uint8_t FADER_STEPS = 25;
uint8_t clearColR, clearColG, clearColB;
uint8_t oldR, oldG, oldB;
uint8_t faderSteps;

//*************************/
// Misc

#define MAX_NR_OF_MODES 10
#define MAX_SLEEP_TIME 160.0f

uint8_t ledPin =  9;
uint8_t oscR, oscG, oscB;
uint8_t mode, modeSave;
int frames=0;

//update strip after DELAY
uint8_t DELAY = 20;
//current delay value
uint8_t delayTodo = 0;


//use serail debug or not
#define USE_SERIAL_DEBUG 1

//uncomment it to enable audio
#define USE_AUDIO_INPUT 1

//some common color defines
const uint32_t WHITE_COLOR = 0xffffff;

//*************************/
// Audio input
#ifdef USE_AUDIO_INPUT
  boolean isAudioVolumeEnabled;
  float audioVol;
  uint16_t maxVal;
#endif

//reset the arduino
void(* resetFunc) (void) = 0; //declare reset function @ address 0

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

 //
 byte swapCables = EEPROM.read(0);
 if (swapCables == 66) {
#ifdef USE_SERIAL_DEBUG
  Serial.println("SWP");
#endif
   //swapped start
   strip.updatePins(clockPin, dataPin);
 }
 
 Ethernet.begin(myMac ,myIp); 
 
 oscServer.begin(serverPort);
 
 //set callback function
 oscServer.addCallback(OSC_MSG_SET_R, &oscCallbackR); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_SET_G, &oscCallbackG); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_SET_B, &oscCallbackB); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_SET_DELAY, &oscCallbackDelay); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_CHANGE_MODE, &oscCallbackChangeMode); //PARAMETER: None, just a trigger
 oscServer.addCallback(OSC_MSG_CHANGE_MODE_DIRECT, &oscCallbackChangeModeDirect); //PARAMETER: None, just a trigger
 oscServer.addCallback(OSC_MSG_SWAP_CABELING, &oscCallbackSwapCabeling);

//#ifdef USE_AUDIO_INPUT
 Serial.println("AU");
 oscServer.addCallback(OSC_MSG_AUDIO, &oscCallbackAudio); //PARAMETER: 1, int value 0..1
//#endif
 
 //init effect
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
  
  //check if the effect should be updated or not
  if (delayTodo>0) {
     //delay not finished yet - do not modify the strip but read network messages
     delayTodo--;
     delay(1);
  } else {
    //delay finished, update the strip content
    delayTodo=DELAY;
    
#ifdef USE_AUDIO_INPUT
  loopAudioSensor();
#endif
  
    switch (mode) {
      case 0: //color lines
          loopLines();
          break;
      case 1:  //solid color white
      case 2:  //solid color wheel fader
      case 3:  //solid color random fader
          loopSolid();
          break;
      case 4:
          loopRainbow();   //color wheel aka rainbow
          break;
      case 5: //knight rider, 1 mover
      case 6: //knight rider, 4 movers
      case 7: //knight rider, 8 movers
      case 8: //knight rider, block mode
          loopKnightRider();    
          break;
      case 9:
          loopFader(); //fader
          break;
      //internal mode, fade from one color to another
      case 200:
          faderLoop();
          break;
    }
    strip.show();    
    frames++;
  } 
  
  //reduce osc workaround time, to prevent duplicate messages  
  if (oscCallBackWorkarround>0) {
    oscCallBackWorkarround--;
  }
}

void initMode() {
#ifdef USE_SERIAL_DEBUG
  Serial.print("SP ");
#endif  
  
  switch (mode) {
    case 0:
          setupLines();
          break;
    case 1:  
          setupSolid(0);
          break;
    case 2:
          setupSolid(1);
          break;
    case 3:
          setupSolid(2);    
          break;
    case 4:
          setupRainbow();    
          break;
    case 5:
          setupKnightRider(strip.numPixels()/10, 1, 0);    
          break;
    case 6:
          setupKnightRider(strip.numPixels()/10, 4, 0);    
          break;
    case 7:
          setupKnightRider(2, 8, 0);    
          break;
    case 8:
          setupKnightRider(1, 0, 1);    
          break;
    case 9:
          setupFader();
          break;          
  }  
  
#ifdef USE_SERIAL_DEBUG
  Serial.print("M:");
  Serial.println(mode);
#endif  
}

