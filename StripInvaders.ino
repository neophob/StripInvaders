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
// Defines

//use serail debug or not
#define USE_SERIAL_DEBUG 1

//use DHCP server OR static IP. Using DHCP and static ip as fallback is not possible, too less space left on arduino ethernet
#define USE_DHCP 1

//uncomment it to enable audio
//#define USE_AUDIO_INPUT 1

//some common color defines
const uint32_t WHITE_COLOR = 0xffffff;

//*************************/
// Fader

const uint8_t FADER_STEPS = 25;
uint8_t clearColR, clearColG, clearColB;
uint8_t oldR, oldG, oldB;
uint8_t faderSteps;

//*************************/
// WS2801
//how many pixels, I use 32 pixels/m
#define NR_OF_PIXELS 160
#define OSC_WORKARROUND_TIME 2

//output pixels dni:3/2
int dataPin = 3; 
int clockPin = 2;  

//dummy init the ws2801 lib
WS2801 strip = WS2801(NR_OF_PIXELS, dataPin, clockPin); 

//*************************/
// Network settings

#ifndef USE_DHCP
  byte myIp[]  = { 192, 168, 1, 111 };
#endif

byte myMac[] = { 0x00, 0x00, 0xAF, 0xFE, 0xBE, 0x01 };


int serverPort  = 10000;
byte oscCallBackWorkarround;
OSCServer oscServer;


//*************************/
// Misc

#define MAX_NR_OF_MODES 15
#define MAX_SLEEP_TIME 160.0f

uint8_t ledPin = 9;
uint8_t oscR, oscG, oscB;
uint8_t mode, modeSave;
int frames=0;

const byte CONST_I = 'I';
const byte CONST_N = 'N';
const byte CONST_V = 'V';

//update strip after DELAY
uint8_t DELAY = 20;
//current delay value
uint8_t delayTodo = 0;

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
  Serial.println("INV!");
#endif

 //check if data/clk port is stored in the eeprom. First check for header INV 
 int header1 = EEPROM.read(0);
 int header2 = EEPROM.read(1);
 int header3 = EEPROM.read(2);
 
 if (header1 == CONST_I && header2 == CONST_N && header3 == CONST_V) {
   //read data and clk pin from the eeprom
   dataPin = EEPROM.read(3);
   clockPin = EEPROM.read(4);
   strip.updatePins(dataPin, clockPin);
 }
 
#ifdef USE_SERIAL_DEBUG
  Serial.print("D:");
  Serial.print(dataPin, DEC);
  Serial.print(" C:");
  Serial.println(clockPin, DEC);
#endif

 //ws2801 start strips 
 strip.begin();
 

//DHCP, hint: we cannot use DHCP and manual IP together, out of space!
#ifdef USE_DHCP
 //start Ethernet library using dhcp
 if (Ethernet.begin(myMac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
#else
//Manual IP
  Ethernet.begin(myMac, myIp)
#endif

#ifdef USE_SERIAL_DEBUG 
  Serial.print("IP:");////32818
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
#endif

 oscServer.begin(serverPort);
 
 //set callback function
 oscServer.addCallback(OSC_MSG_SET_R, &oscCallbackR); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_SET_G, &oscCallbackG); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_SET_B, &oscCallbackB); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_SET_DELAY, &oscCallbackDelay); //PARAMETER: 1, float value 0..1
 oscServer.addCallback(OSC_MSG_CHANGE_MODE, &oscCallbackChangeMode); //PARAMETER: None, just a trigger
 oscServer.addCallback(OSC_MSG_CHANGE_MODE_DIRECT, &oscCallbackChangeModeDirect); //PARAMETER: None, just a trigger
 oscServer.addCallback(OSC_MSG_SWAP_CABELING, &oscCallbackSwapCabeling);

#ifdef USE_AUDIO_INPUT
 Serial.println("AU");
 oscServer.addCallback(OSC_MSG_AUDIO, &oscCallbackAudio); //PARAMETER: 1, int value 0..1
#endif
 
 //init effect
 setupLines();
 
 pinMode(ledPin, OUTPUT);  
 
 //we-are-ready indicator
 synchronousBlink();
 delay(50);
 synchronousBlink();
 delay(50);
 synchronousBlink();

  // Initialize the Bonjour/MDNS library. You can now reach or ping this
  // Arduino via the host name "Invader.local", provided that your operating
  // system is Bonjour-enabled (such as MacOS X).
  EthernetBonjour.begin("Invader");
  
  // Now let's register the service we're offering (a web service) via Bonjour!
  // With the service registered, it will show up in a Bonjour-enabled web
  // browser. As an example, if you are using Apple's Safari, you will now see
  // the service under Bookmarks -> Bonjour (Provided that you have enabled
  // Bonjour in the "Bookmarks" preferences in Safari).
  int ret = EthernetBonjour.addServiceRecord("Invader._osc", 10000, MDNSServiceUDP);  
  if (ret==0) {
    //error, bonjour service failed
  }
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
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
          loopXor(); //fader
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
          setupXor(0);
          break;          
    case 10:
          setupXor(1);
          break;          
    case 11:
          setupXor(2);
          break;          
    case 12:
          setupXor(3);
          break;          
    case 13:
          setupXor(4);
          break;          
    case 14:
          setupXor(5);
          break;          
  }  

#ifdef USE_SERIAL_DEBUG
  Serial.print("M:");
  Serial.println(mode);
#endif  
}



