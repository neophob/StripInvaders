#ifdef USE_AUDIO_INPUT

const int numReadings = 8;
int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

void loopAudioSensor() {
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = analogRead(A0); 
  // add the reading to the total:
  total= total + readings[index++];       
  // advance to the next position in the array:         

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;         

  if (average > maxVal) {
    maxVal = average;
  }
  
  //calculate current volume vs. maximal volume
  audioVol = (1.0f/maxVal)*average;    
  
  //decrease maximum volume, adjust it
  if (maxVal>2) {
    maxVal-=2;  
  } else if (maxVal>1) {
    maxVal--; 
  }
  
/*
#ifdef USE_SERIAL_DEBUG
  Serial.print("avg:");
  Serial.print(average, DEC);
  Serial.print("  \tav:");
  Serial.println(audioVol, DEC);
#endif
*/
}

#endif

