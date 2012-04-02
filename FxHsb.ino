/******************************************************************************
 * accepts hue, saturation and brightness values and outputs three 8-bit color
 * values in an array (color[])
 *
 * saturation (sat) and brightness (bright) are 8-bit values.
 *
 * hue (index) is a value between 0 and 767. hue values out of range are
 * rendered as 0.
 *
 * http://www.johngineer.com/blog/?p=1022
 *
 *****************************************************************************/
/*uint32_t hsb2rgb(uint16_t index, uint8_t sat, uint8_t bright) {
  
	uint16_t r_temp, g_temp, b_temp;
	uint8_t index_mod;
	uint8_t inverse_sat = (sat ^ 255);

	index = index % 768;
	index_mod = index % 256;

	if (index < 256)
	{
		r_temp = index_mod ^ 255;
		g_temp = index_mod;
		b_temp = 0;
	}

	else if (index < 512)
	{
		r_temp = 0;
		g_temp = index_mod ^ 255;
		b_temp = index_mod;
	}

	else if ( index < 768)
	{
		r_temp = index_mod;
		g_temp = 0;
		b_temp = index_mod ^ 255;
	}

	else
	{
		r_temp = 0;
		g_temp = 0;
		b_temp = 0;
	}

	r_temp = ((r_temp * sat) / 255) + inverse_sat;
	g_temp = ((g_temp * sat) / 255) + inverse_sat;
	b_temp = ((b_temp * sat) / 255) + inverse_sat;

	r_temp = (r_temp * bright) / 255;
	g_temp = (g_temp * bright) / 255;
	b_temp = (b_temp * bright) / 255;

        return Color(r_temp, g_temp, b_temp);
}
*/
uint32_t hsb2rgbAN2(uint16_t index, uint8_t sat, uint8_t bright) {
    uint8_t temp[5], n = (index >> 8) % 3;
// %3 not needed if input is constrained, but may be useful for color cycling and/or if modulo constant is fast
    uint8_t x = ((((index & 255) * sat) >> 8) * bright) >> 8;
// shifts may be added for added speed and precision at the end if fast 32 bit calculation is available
    uint8_t s = ((256 - sat) * bright) >> 8;
    temp[0] = temp[3] =              s;
    temp[1] = temp[4] =          x + s;
    temp[2] =          bright - x    ;

    return Color(temp[n + 2], temp[n + 1], temp[n]);
}

void loopHsb() {
  for (int i=0; i < strip.numPixels(); i++) {
//    setTintPixelColor(i, hsb2rgbAN2(frames, i+frames, i+frames) );
    setTintPixelColor(i, hsb2rgbAN2(frames, i+frames, 255) );
  }    
}
