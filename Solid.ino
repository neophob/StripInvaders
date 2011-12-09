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
  for (int i=0; i < strip.numPixels(); i++) {
    switch (solidmode) {
      case 0:
          setTintPixelColor(i, WHITE_COLOR);
          break;
      case 1:
          setTintPixelColor(i, Wheel(frames%255));
          break;
    }      
  }
}

