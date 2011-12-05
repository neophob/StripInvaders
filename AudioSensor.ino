
//TODO: maybe we need to smooth the values (arduino example)
void loopAudioSensor() {
  uint16_t sensorValue = analogRead(A0);//use A0 to read the electrical signal
  if (sensorValue > maxVal) {
    maxVal = sensorValue;
  }
  
  //calculate current volume vs. maximal volume
  audioVol = (1.0f/maxVal)*sensorValue;
  
  //decrease maximum volume, adjust it
  if (maxVal>1) {
    maxVal--;  
  }
  
#ifdef USE_SERIAL_DEBUG
  Serial.print("sensorValue: ");
  Serial.println(sensorValue, DEC);
  Serial.print("audioVol: ");
  Serial.println(audioVol, DEC);
#endif 

}
