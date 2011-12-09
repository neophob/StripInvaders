//Just a quick hack to create effects for StripInvaders, (c) by Michael Vogt / neophob.com

// constants
static final int LEDS = 160;
static final int LEDSIZE = 5;

color led[] = new color[LEDS];
int frame=0;

/*
EFFECT VARIABLES HERE
*/
int krSize = 16;
int kr=0;
int krDirection=0;
color col;

int pixelsPerBlock = 10;
int blockSize = 16;

void setup(){
  size(LEDS*(LEDSIZE+1), 50);  
  frameRate(40);
  
  noStroke();
  background(0);
}

void draw() {
  
/* EFFECT START HERE */  

//BLOCK
/*  int ofs=0;
  for (int x=0; x<blockSize; x++) {
    color cc = Harmony((frame+x<<2)%255);
    for (int i=0; i<pixelsPerBlock; i++) {
      led[ofs++] = cc;
    }
  }/**/


//RAINBOW
  for (int x=0; x<LEDS; x++) {
//    led[x] = Wheel((frame+x)%255);
    led[x] = Harmony((frame+x)%255);
  }
  
//SOLID  
/*  for (int x=0; x < LEDS; x++) {
    led[x] = Wheel(frame%255);//color(255,255,255);
  }  */
  
//KNIGHT RIDER
/*  for (int i=0; i < LEDS; i++) {
    led[i] = c;
  }
  
  for (int i=kr; i<kr+krSize && i<LEDS; i++) {
    led[i] = color(255,255,255);      
  }
 
  if (krDirection==0) {
    kr++;
  } else {
    kr--;
  }
  
  if (kr>LEDS-krSize) {
    krDirection = 1;
    kr = LEDS-krSize;
  }
  
  if (kr==0) {
   krDirection = 0;
   kr = 0; 
  }  
/**/

/* EFFECT END HERE */

  for (int x=0; x<LEDS; x++) {
    fill(color(led[x]));
    rect(x*(LEDSIZE+1), 10, LEDSIZE, LEDSIZE*3);
  }

  frame++;
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

color Harmony(int WheelPos) {
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
