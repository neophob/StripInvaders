//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//
//Solid color effects

//mode 0: solid white color
//mode 1: wheel color
uint8_t solidmode;

void setupSolid(uint8_t _mode) {
  solidmode = _mode;
}

void loopSolid() {
  
    switch (solidmode) {
      case 0:
          fillAllPixelWith(WHITE_COLOR);
          break;
      case 1:
          fillAllPixelWith(Wheel(frames%255));
          break;
      case 2:
          //we get back to this fx AFTER the whole fading is done!
          startFadeToRandomColor(random(255), random(255), random(255));
          break;
    }      

}


void fillAllPixelWith(uint32_t col) {
  for (uint16_t i=0; i < strip.numPixels(); i++) {
     setTintPixelColor(i, col);
  }
}

