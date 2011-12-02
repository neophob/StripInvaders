//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//
//Solid color effects

void setupSolid() {
}

void loopSolid() {
  for (int i=0; i < strip.numPixels(); i++) {
      setTintPixelColor(i, 0xffffff);
  }
  strip.show(); 
  delay(DELAY);  
  
}

