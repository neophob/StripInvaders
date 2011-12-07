//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//
//some leds are fading in, others are fading out...

//just a constant for the random selection
#define RND 512

struct star {
  uint8_t col;
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
    strip.setPixelColor(i, 0);  
  }
}


void loopStars() {
  
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
      //increase color...
      stars[i].col+=2;      
      setTintPixelColor(i, Color(stars[i].col, stars[i].col, stars[i].col));
      
      if (stars[i].col >= 190+random(64)) {
        stars[i].pos = 2;
      }
    } else 
    // ========
    // FADE OUT
    // ========
    if (stars[i].pos == 2) {
      //decrease color...
      if (stars[i].col>=2) {
        stars[i].col-=2; 
      }

      setTintPixelColor(i, Color(stars[i].col, stars[i].col, stars[i].col));

      if (stars[i].col < 2) {
        //restart
        stars[i].pos = 0;
        strip.setPixelColor(i, 0);  
      }
    }    
  }
  
  strip.show(); 
  delay(DELAY);    
}


//init new star
void initStar(int i) {
    stars[i].col = 0;
    stars[i].pos = 1;
}


