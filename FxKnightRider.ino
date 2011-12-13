//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//
//Knight Rider effects

const uint8_t pixelsPerBlock = 10;
const uint8_t blockSize = 16;

uint16_t kr=0;

byte krDirection=0;
byte krSize;
byte howMany;
byte krMode;

//krSize  = the size of the moving parts in pixel
//howMany = how many movers should be active?
//krMode  = 0:single pixel update, 1: block update
void setupKnightRider(byte _krSize, byte _howMany, byte _krMode) {
  krSize = _krSize;
  howMany = _howMany;
  krMode = _krMode;
}


void loopKnightRider() {
  if (krMode==0) {
    //single pixel mode
    uint32_t clearCol = complementaryColor();  
    for (byte i=0; i < strip.numPixels(); i++) {
      setTintPixelColor(i, clearCol);
    }  
    drawKnightRider();    
  } else {
    //block mode
    uint16_t ofs = kr*pixelsPerBlock;
    uint32_t cc = Wheel((frames+kr)%255);
    for (byte i=0; i<pixelsPerBlock; i++) {
      setTintPixelColor(ofs++, cc);
    }

    if (frames % 8==0) checkSwapDirection(blockSize);

  }
}


//draw nrOf knight rider lines and check for updates
void drawKnightRider() {
  int ofs=0;
  
  for (int n=0; n<howMany; n++) {
    for (int i=kr; i<kr+krSize && i<strip.numPixels(); i++) {
      setTintPixelColor(i+ofs, WHITE_COLOR);
    }
    ofs+=strip.numPixels()/howMany;
  }

  checkSwapDirection(strip.numPixels()/howMany);  
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

