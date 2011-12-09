//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//
//Knight Rider effects

uint16_t kr=0;
byte krDirection=0;
byte krSize;
uint32_t color;

void setupKnightRider() {
  krSize = strip.numPixels()/10;
}

void loopKnightRider() {
  for (byte i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }

  for (int i=kr; i<kr+krSize && i<strip.numPixels(); i++) {
    setTintPixelColor(i, WHITE_COLOR);
  }

  if (krDirection==0) {
    kr++;
  } else {
    kr--;
  }
  
  if (kr>strip.numPixels()-krSize) {
    krDirection = 1;
    kr = strip.numPixels()-krSize;
  }
  
  if (kr==0) {
   krDirection = 0;
   kr = 0; 
  }  
}

