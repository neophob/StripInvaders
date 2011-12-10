//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//
//Knight Rider effects

uint16_t kr=0;
byte krDirection=0;
byte krSize;
byte howMany;


void setupKnightRider(byte _krSize, byte _howMany) {
  krSize = _krSize;
  howMany = _howMany;
}


void loopKnightRider() {
  uint32_t clearCol = complementaryColor();  
  for (byte i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, clearCol);
  }
  
  drawKnightRider(howMany);
}


//draw nrOf knight rider lines and check for updates
void drawKnightRider(int nrOf) {
  int ofs=0;
  
  for (int n=0; n<nrOf; n++) {
    for (int i=kr; i<kr+krSize && i<strip.numPixels(); i++) {
      setTintPixelColor(i, WHITE_COLOR);
    }
    ofs+=strip.numPixels()/nrOf;
  }

  checkSwapDirection(strip.numPixels()/nrOf);  
}

//should the direction swapped?
void checkSwapDirection(int lengthOfStrip) {
  if (krDirection==0) {
    kr++;
  } else {
    kr--;
  }
  
  if (kr>lengthOfStrip-krSize) {
    krDirection = 1;
    kr = lengthOfStrip-krSize;
  }
  
  if (kr==0) {
   krDirection = 0;
   kr = 0; 
  }    
}

