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
int krSize = 16;
int kr=0;
int krDirection=0;
color col;

int pixelsPerBlock = 20;
int blockSize = 8;
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
  /*
if (xxx>pixelsPerBlock) xxx=0;
int ofs=0;
int a = frame%255;
color c1=Wheel(a);
color c2=Wheel(255-a);

  for (int x=0; x<blockSize; x++) {
    for (int i=0; i<pixelsPerBlock; i++) {

      if (i>xxx) {
        led[ofs++] = c1;
      } else {
        led[ofs++] = c2;
      }

    }
  }
  xxx++;*/
  
//BLOCK
/*  int ofs=0;
  for (int x=0; x<blockSize; x++) {
    color cc = Harmony((frame+x<<2)%255);
    for (int i=0; i<pixelsPerBlock; i++) {
      led[ofs++] = cc;
    }
  }/**/


//fadeToNewColor(int(random(255)), int(random(255)), int(random(255)));

//SOLID  
/*  for (int x=0; x < LEDS; x++) {
    led[x] = Wheel(frame%255);//color(255,255,255);
  }  */
  
//KNIGHT RIDER
if (frame%10==0) {
r=int(random(255));
g=int(random(255));
b=int(random(255));
c_=color(255-r,255-g,255-b);

}
for (int i=0; i < LEDS; i++) {
    led[i] = color(r,g,b);//color(255,128,0);
  }
  
    drawKR(4,c_);
/**/

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
