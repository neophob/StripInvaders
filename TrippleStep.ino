
int andMask[3]  = { 0xff, 0xff00, 0xff0000 };

void setupTrippleStep() {
  //clear strip
  for (int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);  
  }
  
}


void loopTrippleStep() {
    
  //clear strip
  for (int i=0; i < strip.numPixels(); i++) {
     setTintPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + frames) % 256) & andMask[i%3] );
   }
  
  
  strip.show(); 
  delay(DELAY);  
}

