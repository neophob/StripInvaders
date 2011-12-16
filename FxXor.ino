
int xorR, xorG, xorB;
uint8_t xormode;

void setupXor(uint8_t _xormode) {
  xormode = _xormode;
}

void loopXor() {
  
  int ofs;
  for (int i=0; i < strip.numPixels(); i++) {    
    ofs = i+frames;
    
    switch (solidmode) {
      case 0:
          xorR = (ofs^4)%256;
          xorG = ((ofs>>1)^i)%256;
          xorB = (ofs^frames)%256;
          break;

      case 1:    
          xorR = (ofs^4)%256;
          xorG = ((ofs>>1)^frame)%256;
          xorB = (r^g)%256;
          break;
          
      case 2:
          xorR = (ofs^i)%256;
          xorG = (ofs^i)%256;
          xorB = (ofs^r)%256;
          break;
          
      case 3:
          xorR = (i*(frame>>1))%256;
          xorG = (i^ofs)%256;
          xorB = (ofs)%256;
          break;
          
      case 4:
          xorR = (frame*i)%256;
          xorG = xorR;
          xorB = xorR;
          break;

      case 5:
          ofs >>= 3;
          xorR = (ofs*ofs)%256;
          xorG = (i*ofs>>1)%256;
          xorB = ((frame>>3)*ofs>>1)%256;
          break;

    }    
    
    setTintPixelColor(i, Color(xorR, xorG, xorB) );
  }
}
