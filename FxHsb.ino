/******************************************************************************
 * accepts hue, saturation and brightness values and outputs three 8-bit color
 * values in an array (color[])
 *
 * saturation (sat) and brightness (bright) are 8-bit values.
 *
 * hue (index) is a value between 0 and 767. hue values out of range are
 * rendered as 0.
 *
 * original code: http://www.johngineer.com/blog/?p=1022, optimize
 *
 *****************************************************************************/

uint32_t hsb2rgbAN2(uint16_t index, uint8_t sat, uint8_t bright) {
    uint8_t temp[3], n = (index >> 8) % 3;
// %3 not needed if input is constrained, but may be useful for color cycling and/or if modulo constant is fast
    uint8_t x = ((((index & 255) * sat) >> 8) * bright) >> 8;
// shifts may be added for added speed and precision at the end if fast 32 bit calculation is available
    uint8_t s = ((256 - sat) * bright) >> 8;
    temp[0] = s;
    temp[1] = x + s;
    temp[2] = bright - x;

    return Color(temp[n + 2], temp[n + 1], temp[n]);
}

void loopHsb() {
  for (uint16_t i=0; i < strip.numPixels(); i++) {
//    setTintPixelColor(i, hsb2rgbAN2(frames, i+frames, i+frames) );
    setTintPixelColor(i, hsb2rgbAN2(frames, i, 255) );
  }    
}

