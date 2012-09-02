//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//
//rainbow fx, most code by ladyada

void setupRainbow() {
  //nothing todo
}

void loopRainbow() {
  for (uint16_t i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
    setTintPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + frames) % 256) );
  }  
}

