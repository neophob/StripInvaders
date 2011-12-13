//*************************/
// OSC callback

// DELAY
void oscCallbackDelay(OSCMessage *_mes){
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;

  //delay between 0ms and 120ms
  DELAY = byte( _mes->getArgFloat(0)*MAX_SLEEP_TIME );

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

  //toggle button
  float arg=_mes->getArgFloat(0);
  if (arg==0.0f) {
    isAudioVolumeEnabled = false;
  } else {
    isAudioVolumeEnabled = true;
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
  
  byte arg=byte(_mes->getArgFloat(0));  
  if (arg > MAX_NR_OF_MODES-1) {
    return;
  }

  mode=arg;
  modeSave = mode;
  initMode(); 
}


// next mode
void increaseMode() {
  if (modeSave<MAX_NR_OF_MODES-1) {
    //incase we are fading (mode 200) we need to use the save value
    mode = modeSave+1;
  } else {
    mode = 0; 
  }
  
  modeSave = mode;  
  initMode();   
}


// change mode, just increase current mode
void oscCallbackChangeMode(OSCMessage *_mes){
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;

  //touchOSC send float! int does NOT work
//  uint8_t arg=_mes->getArgInt32(0) & 0xff;
  float arg=_mes->getArgFloat(0);
  if (arg < 1.f) {
    return;
  }
 
  increaseMode(); 
}


//Swap cabling of ws2801 strips and store config into eeprom
void oscCallbackSwapCabeling(OSCMessage *_mes){
  if (oscCallBackWorkarround>0) return;
  oscCallBackWorkarround = OSC_WORKARROUND_TIME;
  
  int magicByte = _mes->getArgInt32(0);
  int dataPin = _mes->getArgInt32(1);
  int clkPin = _mes->getArgInt32(2);
  
  //66 is just a magic nr.
  if (magicByte == 66) {
    EEPROM.write(0, CONST_I);
    EEPROM.write(1, CONST_N);
    EEPROM.write(2, CONST_V);
    EEPROM.write(3, dataPin);
    EEPROM.write(4, clkPin);
    
#ifdef USE_SERIAL_DEBUG
  Serial.println("RBT");
#endif  
    //just to be sure
    delay(250);
    synchronousBlink();
    delay(50);
    synchronousBlink();

    resetFunc();  //call reset    
  }
    
}

