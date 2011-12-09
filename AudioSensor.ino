#ifdef USE_AUDIO_INPUT

//TODO: maybe we need to smooth the values (arduino example)
void loopAudioSensor() {

  uint16_t sensorValue = analogRead(A0); //use A0 to read the electrical signal
  if (sensorValue > maxVal) {
    maxVal = sensorValue;
  }
  
  //calculate current volume vs. maximal volume
  if (sensorValue>0) {
    audioVol = (1.0f/maxVal)*sensorValue;    
  }
  
  //decrease maximum volume, adjust it
  if (maxVal>2) {
    maxVal-=2;  
  }
  
/*
#ifdef USE_SERIAL_DEBUG
  Serial.print("\rsens:");
  Serial.print(sensorValue, DEC);
  Serial.print("  \tav:");
  Serial.print(audioVol, DEC);
  Serial.print("  \tmx:");
  Serial.println(maxVal, DEC);
#endif 
*/
}

#endif

