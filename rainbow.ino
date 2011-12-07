//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//
//rainbow fx, most code by ladyada

int j=0;

void setupRainbow() {
}

void loopRainbow() {
  int i;
 
  for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
    setTintPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
  }  

  j++;
  strip.show(); 
  delay(DELAY);  
}


//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170; 
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

