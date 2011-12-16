
int xorR, xorG, xorB;

void setupXor() {

}

void loopXor() {
  int ofs;
  for (int i=0; i < strip.numPixels(); i++) {
    ofs = i+frames;
    xorR = (ofs^8)%256;
    xorG = ((ofs>>1)^i)%256;
    xorB = (ofs^frames)%256;
    setTintPixelColor(i, Color(xorR, xorG, xorB) );
  }
}
