//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//
//Knight Rider effects

byte kr=0;
byte krDirection=0;

void setupKnightRider() {
  //nothing todo
}

void loopKnightRider() {
  for (byte i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }

  if (kr<strip.numPixels()-1) {
    setTintPixelColor(kr+1, Color(255, 255, 255));
  }
  setTintPixelColor(kr, Color(255, 255, 255));
  if (kr>1) {
    setTintPixelColor(kr-1, Color(255, 255, 255));
  }
 
  if (krDirection==0) {
    kr++;
  } else {
    kr--;
  }
  
  if (kr>strip.numPixels()-1) {
    krDirection = 1;
    kr = strip.numPixels();
  }
  
  if (kr==0) {
   krDirection = 0;
   kr = 0; 
  }  
}

