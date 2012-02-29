//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // http://pixelinvaders.ch
//Network/OSC Enabled Strip Controller
//Needed 3rd Party Library:
//  -https://github.com/neophob/EthernetBonjour (Bonjour, original code by gkaindl.com)
//  -https://github.com/neophob/ArdOSC (AndOSC, origianl code by recotana.com)
//  -https://github.com/neophob/WS2801-Library (WS2801, original code by adafruit.com)
//   OR
//  -https://github.com/neophob/LPD8806 (LPD8806, original code by adafruit.com)

#include <SPI.h>
#include <Ethernet.h>
#include <ArdOSC.h>
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
#define OSC_MSG_CONFIG "/cfg"
#define OSC_MSG_SAVE "/sav"

//*************************/
// define strip hardware, use only ONE hardware type
#define USE_WS2801 1
//#define USE_LPD8806 1


#ifdef USE_WS2801
  #include <WS2801.h>
#endif
#ifdef USE_LPD8806
  #include <LPD8806.h>
#endif  

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

//EEPROM offset
#define EEPROM_HEADER_1 0
#define EEPROM_HEADER_2 1
#define EEPROM_HEADER_3 2
#define EEPROM_POS_DATA 3
#define EEPROM_POS_CLK 4
#define EEPROM_POS_COUNT 5
#define EEPROM_MAGIC_BYTE 66

#define EEPROM_HEADER_10 10
#define EEPROM_HEADER_11 11
#define EEPROM_HEADER_12 12
#define EEPROM_POS_MODE 13
#define EEPROM_POS_DELAY 14
#define EEPROM_POS_R 15
#define EEPROM_POS_G 16
#define EEPROM_POS_B 17

//output pixels dni:3/2
int dataPin = 3; 
int clockPin = 2;  

//dummy init the pixel lib
#ifdef USE_WS2801
WS2801 strip = WS2801(); 
#endif
#ifdef USE_LPD8806
LPD8806 strip = LPD8806(); 
#endif  

//*************************/
// Network settings

#ifndef USE_DHCP
   byte myIp[]  = { 192, 168, 1, 111 };
#endif

byte myMac[] = { 0x00, 0x00, 0xAF, 0xFE, 0xBE, 0x01 };

const int serverPort  = 10000;
byte oscCallBackWorkarround;
OSCServer oscServer;


//*************************/
// Misc

#define MAX_NR_OF_MODES 15
#define MAX_SLEEP_TIME 160.0f

const uint8_t ledPin = 9;
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

  int cnt = NR_OF_PIXELS;
  
#ifdef USE_SERIAL_DEBUG
  Serial.begin(115200);
  
#ifdef USE_WS2801
  Serial.println("INV2801!");
#endif
#ifdef USE_LPD8806
  Serial.println("INV8806!");
#endif    

#endif

  //check if data/clk port is stored in the eeprom. First check for header INV 
  byte header1 = EEPROM.read(EEPROM_HEADER_1);
  byte header2 = EEPROM.read(EEPROM_HEADER_2);
  byte header3 = EEPROM.read(EEPROM_HEADER_3);

  if (header1 == CONST_I && header2 == CONST_N && header3 == CONST_V) {
    //read data and clk pin from the eeprom
    dataPin = EEPROM.read(EEPROM_POS_DATA);
    clockPin = EEPROM.read(EEPROM_POS_CLK);
    cnt = EEPROMReadInt(EEPROM_POS_COUNT);
    
    //just add some sanity check here, prevent out of memory
    if (cnt<1 || cnt>1024) {
      cnt = NR_OF_PIXELS;
    }
  }

#ifdef USE_SERIAL_DEBUG
  Serial.print("D:");
  Serial.print(dataPin, DEC);
  Serial.print(" C:");
  Serial.print(clockPin, DEC);
  Serial.print("//");
  Serial.println(cnt, DEC);
#endif

#ifdef USE_WS2801
  strip = WS2801(cnt, dataPin, clockPin); 
#endif
#ifdef USE_LPD8806
  strip = LPD8806(cnt, dataPin, clockPin); 
#endif  

  
  //ws2801 start strips 
  strip.begin();


  //DHCP, hint: we cannot use DHCP and manual IP together, out of space!
#ifdef USE_DHCP
  //start Ethernet library using dhcp
  if (Ethernet.begin(myMac) == 0) {
#ifdef USE_SERIAL_DEBUG
    Serial.println("Failed to configure Ethernet using DHCP");
#endif
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
  oscServer.addCallback(OSC_MSG_CONFIG, &oscCallbackConfig);
  oscServer.addCallback(OSC_MSG_SAVE, &oscCallbackSavePreset);  
  

#ifdef USE_AUDIO_INPUT
  Serial.println("AU");
  oscServer.addCallback(OSC_MSG_AUDIO, &oscCallbackAudio); //PARAMETER: 1, int value 0..1
#endif

  //init effect
  initMode();

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
  
  //load presets
  restorePresetStateFromEeprom();

  // Now let's register the service we're offering (a web service) via Bonjour!
  // With the service registered, it will show up in a Bonjour-enabled web
  // browser. As an example, if you are using Apple's Safari, you will now see
  // the service under Bookmarks -> Bonjour (Provided that you have enabled
  // Bonjour in the "Bookmarks" preferences in Safari).
  int ret = EthernetBonjour.addServiceRecord("Invader._osc", serverPort, MDNSServiceUDP);  
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
  } 
  else {
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

/*****************************************************************************************
 *  INIT MODE
 *****************************************************************************************/

void initMode() {

  switch (mode) {
  case 0:
    setupLines();
    break;
  case 1:  
  case 2:
  case 3:
    setupSolid(mode-1);
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
  //save some bytes here, setupXor from (0) till (5)
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:  
    setupXor(mode-9);
    break;          
  }  

#ifdef USE_SERIAL_DEBUG
  Serial.print("M:");
  Serial.println(mode);
#endif  
}




