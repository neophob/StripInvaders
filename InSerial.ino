
//press m on the serial port switch the mode, used for debugging
#ifdef USE_SERIAL_DEBUG 
void serialEvent() {
  while (Serial.available()) {
  // get the new byte:
  char inChar = (char)Serial.read(); 
//  Serial.println(inChar, HEX);
  if (inChar == 0x6d) { //if user pressed 'M'
      increaseMode();
      Serial.println();
    } 
  }
}
#endif 


