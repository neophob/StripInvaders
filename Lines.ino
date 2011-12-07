//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//
//Fancy lines effects

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


void setupLines() {
  //fadeToNewColor(false);
  newAnimation(false); 
}


void loopLines() {
  if ((lines.pos > 0 && lines.pos == lines.del) || lines.pos > strip.numPixels()) {
    newAnimation(true);
  }
  
  if (lines.pos > lines.length) {
    lines.del++;
  } else {
    lines.pos++;      
  }
    
  for (int i=0; i < strip.numPixels(); i++) {
    if (i>=lines.ofs+lines.del && i<lines.ofs+lines.pos) {
      setTintPixelColor(i, lines.col);
    } else {
      setTintPixelColor(i, clearCol);
    }      
  }
  
  strip.show(); 
  delay(DELAY);  
}


//init a new line animation
void newAnimation(boolean sleep) {
  fadeToNewColor(sleep);
  
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
void fadeToNewColor(boolean sleep) {
  uint8_t oldR = clearColR;
  uint8_t oldG = clearColG;
  uint8_t oldB = clearColB;

  clearColR = random(70);
  clearColG = random(70);
  clearColB = random(90);
  clearCol = Color(clearColR, clearColG, clearColB);
  
  int steps = 25;
  float stepsR = (clearColR-oldR)/(float)steps;
  float stepsG = (clearColG-oldG)/(float)steps;
  float stepsB = (clearColB-oldB)/(float)steps;

  for (int s=0; s<steps+1; s++) {
    uint8_t rr=oldR+stepsR*s;    
    uint8_t gg=oldG+stepsG*s;
    uint8_t bb=oldB+stepsB*s;
    uint32_t c = Color(rr, gg, bb);
    
    for (int i=0; i < strip.numPixels(); i++) {
      setTintPixelColor(i, c);
    }
    
    if (sleep) {
      strip.show(); 
      delay(DELAY);      
    }
  }
  
}
