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

// get the complementary color of the current tint color
uint32_t complementaryColor() {
  uint32_t c;
  c = 255-oscR;
  c <<= 8;
  c |= 255-oscG;
  c <<= 8;
  c |= 255-oscB;
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

//initialize a new fading from clearCol to random
void startFadeToRandomColor(uint8_t r, uint8_t g, uint8_t b) {
  //save current color
  oldR = clearColR;
  oldG = clearColG;
  oldB = clearColB;

  //define new color
  clearColR = random(r);
  clearColG = random(g);
  clearColB = random(b);
  
  modeSave = mode;
  mode = 200;  
  faderSteps = 0;
}

//fade currentbackground color to next, random color
void faderLoop() {
    float stepsR = (clearColR-oldR)/(float)FADER_STEPS;
    float stepsG = (clearColG-oldG)/(float)FADER_STEPS;
    float stepsB = (clearColB-oldB)/(float)FADER_STEPS;

    uint8_t rr=oldR+stepsR*faderSteps;    
    uint8_t gg=oldG+stepsG*faderSteps;
    uint8_t bb=oldB+stepsB*faderSteps;
    uint32_t c = Color(rr, gg, bb);
    
    for (int i=0; i < strip.numPixels(); i++) {
      setTintPixelColor(i, c);
    }
    
    if (faderSteps++>=FADER_STEPS) {
      mode = modeSave;
    }    
}

//just blink
void synchronousBlink() {
  digitalWrite(ledPin, HIGH);
  delay(20);
  digitalWrite(ledPin, LOW);  
}

