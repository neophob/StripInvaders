//Just a quick hack to create effects for StripInvaders, (c) by Michael Vogt / neophob.com

// constants
static final int LEDS = 160;
static final int LEDSIZE = 5;
static final float FADER_STEPS = 25;

color led[] = new color[LEDS];
int frame=0;

int clearColR;
int clearColG;
int clearColB;
int clearCol;

/*
EFFECT VARIABLES HERE
*/
int krSize = 1;
int kr=0;
int krDirection=0;
color col;

int pixelsPerBlock = 10;
int blockSize = 16;
int xxx=0;

void setup(){
  size(LEDS*(LEDSIZE+1), 50);  
  frameRate(40);
  
  noStroke();
  background(0);
}
int r,g,b;
color c_;

void draw() {

/* EFFECT START HERE */  

//int pixelsPerBlock = 10;
//int blockSize = 16;
int r,g,b;
  for (int i=0; i<LEDS; i++) {
    int ofs = i+frame;
    r = (ofs^8)%256;
    g = ((ofs>>1)^8)%256;
    b = (ofs&4)%256;
    print(i+":"+r+"/"+"g"+"/"+b+" ");
    led[i] = color(r,g,b);
  }
 println(); 
  
//BLOCK
/*  int ofs=kr*pixelsPerBlock;
  color cc = Wheel((frame+kr)%255);
  for (int i=0; i<pixelsPerBlock; i++) {
    led[ofs++] = cc;
  }

  if (frame%8==0) updateKR(blockSize);
  
/**/

 // faderTo(int(random(255)), int(random(255)), int(random(255)), int(random(255)), int(random(255)), int(random(255)));  

/* EFFECT END HERE */

  for (int x=0; x<LEDS; x++) {
    fill(color(led[x]));
    rect(x*(LEDSIZE+1), 10, LEDSIZE, LEDSIZE*3);
  }

  frame++;
}

void drawKR(int nrOf, color c) {
  int ofs=0;
  for (int n=0; n<nrOf; n++) {
    for (int i=kr; i<kr+krSize && i<LEDS; i++) {
      led[i+ofs] = c;      
    }
    ofs+=LEDS/nrOf;
  }

  updateKR(LEDS/nrOf);  
}

void updateKR(int lengthOfStrip) {
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


//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
color Wheel(int WheelPos) {
  if (WheelPos < 85) {
    return color(WheelPos * 3, 255 - WheelPos * 3, 0);  
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170; 
    return color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void faderTo(int s, int e, int r, int g, int b, int r2, int g2, int b2) {
    float stepsR = (r2-r)/float(LEDS);
    float stepsG = (g2-g)/float(LEDS);
    float stepsB = (b2-b)/float(LEDS);

    for (int i=s; i < e; i++) {
      int rr=int(r+stepsR*i);
      int gg=int(g+stepsG*i);
      int bb=int(b+stepsB*i);

      led[i] = color(rr, gg, bb);
    }
    delay(111);
}

void fadeToNewColor(int r, int g, int b) {
  int oldR = clearColR;
  int oldG = clearColG;
  int oldB = clearColB;
//println("---");
  clearColR = int(random(r));
  clearColG = int(random(g));
  clearColB = int(random(b));
  clearCol = color(clearColR, clearColG, clearColB);
  
  float stepsR = (clearColR-oldR)/FADER_STEPS;
  float stepsG = (clearColG-oldG)/FADER_STEPS;
  float stepsB = (clearColB-oldB)/FADER_STEPS;

  for (int s=0; s<FADER_STEPS+1; s++) {
    int rr=int(oldR+stepsR*s);    
    int gg=int(oldG+stepsG*s);
    int bb=int(oldB+stepsB*s);
    int c = color(rr, gg, bb);
    
    for (int i=0; i < LEDS; i++) {
      led[i] = c;
    }
    
    for (int x=0; x<LEDS; x++) {
      fill(color(led[x]));
      rect(x*(LEDSIZE+1), 10, LEDSIZE, LEDSIZE*3);
    }
    //println("fade to "+c);
delay(40);
  }
}
