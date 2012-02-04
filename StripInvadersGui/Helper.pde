private void ping() {
  updateTextfield("Ping \""+HOSTNAME+"\"", false);
  try {
    stripInvader = new NetAddress(HOSTNAME, OSC_PORT);
    updateTextfield(", reachable: "+stripInvader.isvalid());
    if (stripInvader.isvalid()) {
      unlockGui();
    }
  } 
  catch (Exception e) {
    updateTextfield(", reachable: false "+e);
  }
}

//set default config
private void doStripInvaderDefaultConfig() { 
  dataPin = 2; 
  clockPin = 3;
  ledCount = 160;
  /*
Hint, here is the code stripinvaders use to read the osc message
   int magicByte = _mes->getArgInt32(0);
   int dataPin = _mes->getArgInt32(1);
   int clkPin = _mes->getArgInt32(2);
   int cnt = _mes->getArgInt32(3);
   */
}

private void sendConfiguration() {
  clearTextfield();

  //sanity checks
  if (stripInvader == null || !stripInvader.isvalid()) {
    updateTextfield("ERROR: StripInvader not found! Make sure its connected and your DHCP Server is functional.");
    //return;
  }

  if (dataPin==clockPin) {
    updateTextfield("ERROR: Data Pin and Clock Pin cannot be equal!");
    return;
  }

  updateTextfield("Config, dataPin: "+dataPin+", clockPin: "+clockPin+", LED count: "+ledCount);
  updateTextfield("Try to send configuration to "+HOSTNAME+" at OSC Port "+OSC_PORT);

  try {
    OscMessage configMessage = new OscMessage("/cfg");
    configMessage.add(OSC_MAGIC); /* add magic byte */
    configMessage.add(dataPin); /* add data pin */
    configMessage.add(clockPin); /* add clock pin */
    configMessage.add(ledCount); /* add led count */
    
    OscP5 oscP5 = new OscP5(this, 0);
    oscP5.send(configMessage, stripInvader);
    oscP5.disconnect(stripInvader);
    oscP5.dispose();
    
    updateTextfield("StripInvader updated!");

  } 
  catch (Exception e) {
    updateTextfield("Failed to send message: "+e);
  }
}

