
void loopAudioSensor() {
  uint16_t sensorValue = analogRead(A0);//use A0 to read the electrical signal
  if (sensorValue > maxVal) {
    maxVal = sensorValue;
  }
  
  audioVol = (1.0f/maxVal)*sensorValue;
  
#ifdef USE_SERIAL_DEBUG
  Serial.print("sensorValue: ");
  Serial.println(sensorValue, DEC);
  Serial.print("audioVol: ");
  Serial.println(audioVol, DEC);
#endif 

}
