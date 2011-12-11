
uint8_t r,g,b,r2,g2,b2;

void setupFader() {
  r = random(255);
  g = random(255);
  b = random(255);
  r2 = random(255);
  g2 = random(255);
  b2 = random(255);
}

uint8_t update(uint8_t val, int8_t add) {
  int16_t tmp = val;
  tmp += add;
  if (tmp>255) tmp=255;
  if (tmp<0) tmp=0;
  return (uint8_t) tmp;
}


void loopFader() {
  int8_t rnd = random(21)-10;
  
  switch (random(6)) {
    case 0:
          r = update(r,rnd);
          break;
    case 1:  
          g = update(g,rnd);
          break;
    case 2:  
          b = update(b,rnd);
          break;
    case 3:  
          r2 = update(r2,rnd);
          break;
    case 4:  
          g2 = update(g2,rnd);
          break;
    case 5:  
          b2 = update(b2,rnd);
          break; 
  }
  
  faderTo(r, g, b, r2, g2, b2);
}
