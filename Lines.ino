//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//
//Fancy lines effects

const uint8_t FADER_STEPS = 25;
int andMask[3];

struct line {
  uint8_t ofs;
  uint8_t pos;
  uint8_t del;
  uint8_t length;
  uint32_t col;
};
line lines;

uint8_t clearColR;
uint8_t clearColG;
uint8_t clearColB;
uint32_t clearCol;

//init effect
void setupLines(boolean fancyMode) {
  if (fancyMode) {
    andMask[0] = 0x0000ff; 
    andMask[1] = 0x00ff00;
    andMask[2] = 0xff0000;
  } else {
    andMask[0] = 0xffffff;
    andMask[1] = 0xffffff;
    andMask[2] = 0xffffff;    
  }
  newAnimation(); 
}

//main loop
void loopLines() {
  if ((lines.pos > 0 && lines.pos == lines.del) || lines.pos > strip.numPixels()) {
    fadeToNewColor();
    newAnimation();
  }
  
  if (lines.pos > lines.length) {
    lines.del++;
  } else {
    lines.pos++;      
  }
    
  for (int i=0; i < strip.numPixels(); i++) {
    if (i>=lines.ofs+lines.del && i<lines.ofs+lines.pos) {
      setTintPixelColor(i, lines.col & andMask[i%3]);
    } else {
      setTintPixelColor(i, clearCol & andMask[i%3]);
    }      
  }  
}


//init a new line animation
void newAnimation() {
  lines.length = 0;
  while (lines.length<16) {
    lines.ofs = random(strip.numPixels()); 
    lines.length = random( strip.numPixels()-lines.ofs ); 
  }
  lines.pos = 0;
  lines.del = 0;
  lines.col = Color(random(200), random(200), random(250));  
}


//fade currentbackground color to next, random color
void fadeToNewColor() {
  uint8_t oldR = clearColR;
  uint8_t oldG = clearColG;
  uint8_t oldB = clearColB;

  clearColR = random(70);
  clearColG = random(70);
  clearColB = random(90);
  clearCol = Color(clearColR, clearColG, clearColB);
  
  float stepsR = (clearColR-oldR)/(float)FADER_STEPS;
  float stepsG = (clearColG-oldG)/(float)FADER_STEPS;
  float stepsB = (clearColB-oldB)/(float)FADER_STEPS;

  for (uint8_t s=0; s<FADER_STEPS+1; s++) {
    uint8_t rr=oldR+stepsR*s;    
    uint8_t gg=oldG+stepsG*s;
    uint8_t bb=oldB+stepsB*s;
    uint32_t c = Color(rr, gg, bb);
    
    for (int i=0; i < strip.numPixels(); i++) {
      setTintPixelColor(i, c & andMask[i%3]);
    }
    
    strip.show(); 
    delay(DELAY);      
  }
  
}
