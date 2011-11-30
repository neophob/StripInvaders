
//just a constant for the random selection
#define RND 255

struct star {
  uint32_t currentCol;
  uint32_t endCol;
  //1 = increase to endCol, 2 = decrease to 0
  uint8_t pos;
};

star stars[NR_OF_PIXELS];

void setupStars() {
  for (int i=0; i < strip.numPixels(); i++) {
    if (random(RND)==2) {
      initStar(i);
    } else {
      stars[i].pos = 0;
    }      
  }
}

void loopStars() {
  uint8_t currentR, currentG, currentB;
  uint8_t endR, endG, endB;
  
  for (int i=0; i < strip.numPixels(); i++) {
    
    // ====
    // INIT
    // ====
    if (stars[i].pos == 0) {
      if (random(RND)==2) {
        initStar(i);
      }
    } else

    // =======
    // FADE IN
    // =======
    if (stars[i].pos == 1) {
      
      //decrease color...
      uint32_t ccol = stars[i].currentCol;
      currentB = ccol & 0xff;
      ccol >>= 8;
      currentG = ccol & 0xff;
      ccol >>= 8;
      currentR = ccol & 0xff;

      uint32_t ecol = stars[i].endCol;
      endB = ecol & 0xff;
      ecol >>= 8;
      endG = ecol & 0xff;
      ecol >>= 8;
      endR = ecol & 0xff;
      
      if (currentR<endR) currentR+=2; else currentR=endR;
      if (currentG<endG) currentG+=2; else currentG=endG;
      if (currentB>endB) currentB+=2; else currentB=endB;
      
      if (currentR==endR && currentG==endG && currentB == endB) {
        stars[i].pos = 2;
      }

      stars[i].currentCol = Color(currentR, currentG, currentB);      
    } else 

    // ========
    // FADE OUT
    // ========
    if (stars[i].pos == 2) {
      //decrease color...
      uint32_t ccol = stars[i].currentCol;
      currentB = ccol & 0xff;
      ccol >>= 8;
      currentG = ccol & 0xff;
      ccol >>= 8;
      currentR = ccol & 0xff;

      if (currentR>2) currentR-=3; else currentR=0;
      if (currentG>2) currentG-=3; else currentG=0;
      if (currentB>2) currentB-=3; else currentB=0;

      stars[i].currentCol = Color(currentR, currentG, currentB);

      if (stars[i].currentCol == 0) {
        stars[i].pos = 0;
      }
    }
    
    //update color
    strip.setPixelColor(i, stars[i].currentCol);
  }
  
  strip.show(); 
  delay(DELAY);  
}


//init new star
void initStar(int i) {
    uint8_t rnd = random(192);
    uint8_t rnd2 = random(64);
    stars[i].endCol = Color(rnd+rnd2, rnd+rnd2, rnd2);
    stars[i].pos = 1;
    stars[i].currentCol=0;  
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
