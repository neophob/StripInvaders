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


//init effect
void setupLines() {
  newAnimation(); 
}

//main loop
void loopLines() {
  if ((lines.pos > 0 && lines.pos == lines.del) || lines.pos > strip.numPixels()) {
    startFadeToRandomColor(128, 128, 128);
    newAnimation();
    return;
  }
  
  if (lines.pos > lines.length) {
    lines.del++;
  } else {
    lines.pos++;      
  }
   
  uint32_t clearCol = Color(clearColR, clearColG, clearColB); 
  for (int i=0; i < strip.numPixels(); i++) {
    if (i>=lines.ofs+lines.del && i<lines.ofs+lines.pos) {
      setTintPixelColor(i, lines.col);
    } else {
      setTintPixelColor(i, clearCol);
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
  lines.col = Color(random(220), random(220), random(255));  
}


