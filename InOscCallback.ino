//*************************/
// OSC callback

// DELAY
void oscCallbackDelay(OSCMessage *_mes){
  //delay between 0ms and 120ms
  DELAY = byte( _mes->getArgFloat(0)*MAX_SLEEP_TIME );

#ifdef USE_SERIAL_DEBUG
  Serial.print("D:");
  Serial.println(DELAY, DEC);
#endif 
}

//convert the first osc argument and return byte
byte getFirstFloatArgument(OSCMessage *_mes) {
  return byte( _mes->getArgFloat(0)*255.f );
}

// R
void oscCallbackR(OSCMessage *_mes){
  oscR = getFirstFloatArgument(_mes);

#ifdef USE_SERIAL_DEBUG
  Serial.print("R:");
  Serial.println(oscR, DEC);
#endif 
}


// G
void oscCallbackG(OSCMessage *_mes){
  oscG = getFirstFloatArgument(_mes);

#ifdef USE_SERIAL_DEBUG
  Serial.print("G:");
  Serial.println(oscG, DEC);
#endif
}


// B
void oscCallbackB(OSCMessage *_mes){
  oscB = getFirstFloatArgument(_mes);

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
  } 
  else {
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
  mode = 0; 
  if (modeSave<MAX_NR_OF_MODES-1) {
    //incase we are fading (mode 200) we need to use the save value
    mode = modeSave+1;
  } 

  modeSave = mode;  
  initMode();   
}


// change mode, just increase current mode
void oscCallbackChangeMode(OSCMessage *_mes){
  //touchOSC send float! int does NOT work
  //  uint8_t arg=_mes->getArgInt32(0) & 0xff;
  float arg=_mes->getArgFloat(0);
  if (arg < 1.f) {
    //check also for int parameter here!
    if (_mes->getArgInt32(0) != 1) {
      return;
    }
  }

  increaseMode(); 
}


//Swap cabling of ws2801 strips and store config into eeprom
void oscCallbackConfig(OSCMessage *_mes){
  byte magicByte = _mes->getArgInt32(0);
  byte dataPin = _mes->getArgInt32(1);
  byte clkPin = _mes->getArgInt32(2);
  int cnt = _mes->getArgInt32(3);
#ifdef USE_SERIAL_DEBUG
  Serial.println(cnt, DEC);
#endif  

  //66 is just a magic nr.
  if (magicByte == EEPROM_MAGIC_BYTE && dataPin>0 && clkPin>0 && cnt>0 && dataPin!=clkPin) {
    EEPROM.write(EEPROM_HEADER_1, CONST_I);
    EEPROM.write(EEPROM_HEADER_2, CONST_N);
    EEPROM.write(EEPROM_HEADER_3, CONST_V);
    EEPROM.write(EEPROM_POS_DATA, dataPin);
    EEPROM.write(EEPROM_POS_CLK, clkPin);
    //write two bytes
    EEPROMWriteInt(EEPROM_POS_COUNT, cnt);

#ifdef USE_SERIAL_DEBUG
    Serial.println("RBT");
#endif  

    //just to be sure
    delay(50);
    synchronousBlink();

    resetFunc();  //call reset    
  } 
  else {

#ifdef USE_SERIAL_DEBUG
    Serial.println("Invalid");
#endif      
  }    
}


void oscCallbackSavePreset(OSCMessage *_mes){
  saveCurrentStateToEeprom();
}


void oscCallbackPixel(OSCMessage *_mes){
  int16_t argCount = _mes->getArgsNum();
  if (argCount<5) {
#ifdef USE_SERIAL_DEBUG
    Serial.print("Invalid Parametercount: ");
    Serial.println(argCount, DEC);
#endif     
    return;
  }
  
  //change mode to serverimage - else buffer would be overwritten!
  //oscMode = MODE_SERVER_IMAGE;
  
  int16_t ofs = _mes->getArgInt32(0);
#ifdef USE_SERIAL_DEBUG
    Serial.print("OSC Update Pixel, offset: ");
    Serial.println(ofs, DEC);
#endif     
  
  //get 4 colors from osc message
  for (byte b=0; b<4; b++) {
    int32_t col=_mes->getArgInt32(b+1);
    strip.setPixelColor(ofs+b, col);
#ifdef USE_SERIAL_DEBUG
    Serial.print("Set: ");
    Serial.println(col, HEX);
#endif     
  }
  
}


