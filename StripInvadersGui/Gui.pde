
void initGui() {
  cp5 = new ControlP5(this);

  myTextarea = cp5.addTextarea("txt")
    .setPosition(20, 220)
      .setSize(560, 80)
        .setFont(createFont("arial", 12))
          .setLineHeight(14)
            .setColor(color(192))
              .setColorBackground(color(255, 100))
                .setColorForeground(color(255, 100));

  btnPing = cp5.addButton("Find StripInvader Controller")
    .setValue(0)
      .setPosition(20, 20)
        .setSize(200, 20)
          ;

  sData= cp5.addSlider("Data Pin")
    .setPosition(20, 50)
      .setSize(200, 20)
        .setRange(0, 15)
          .setValue(dataPin)
            .setDecimalPrecision(0)
              .setNumberOfTickMarks(16)
                .snapToTickMarks(true)
                  .lock()
                    ;

  sClk= cp5.addSlider("Clock Pin")
    .setPosition(20, 80)
      .setSize(200, 20)
        .setRange(0, 15)
          .setValue(clockPin)
            .setDecimalPrecision(0)
              .setNumberOfTickMarks(16)
                .snapToTickMarks(true)
                  .lock()
                    ;

  sCount= cp5.addSlider("LED Count")
    .setPosition(20, 110)
      .setSize(400, 20)
        .setRange(8, 400)
          .setValue(ledCount)
            .setDecimalPrecision(0)
              .lock()
                ;

  btnDefault = cp5.addButton("Set default Configuration")
    .setValue(0)
      .setPosition(20, 140)
        .setSize(200, 20)
          .lock()
            ;

  btnConfig = cp5.addButton("Send Configuration")
    .setValue(0)
      .setPosition(20, 190)
        .setSize(200, 20)
          ;

  cp5.addTextlabel("logo", "StripInvader Config v0.1", 205, 310).setFont(ControlP5.synt24);
}

//enable user input
void unlockGui() {
  sData.unlock();
  sClk.unlock();
  sCount.unlock();
  btnDefault.unlock();
}

void controlEvent(ControlEvent theEvent) {
  if (theEvent.isFrom(btnPing)) {
    ping();
    return;
  } 
  else
    if (theEvent.isFrom(sData)) {
      dataPin = int(sData.getValue());
      return;
    } 
    else
      if (theEvent.isFrom(sClk)) {
        clockPin = int(sClk.getValue());
        return;
      } 
      else
        if (theEvent.isFrom(sCount)) {
          ledCount = int(sCount.getValue());
          return;
        }
        else 
          if (theEvent.isFrom(btnConfig)) {
          sendConfiguration();
          return;
        }
        else 
          if (theEvent.isFrom(btnDefault)) {
          doStripInvaderDefaultConfig();
          sData.setValue(dataPin);
          sClk.setValue(clockPin);
          sCount.setValue(ledCount);
          return;
        }
}

void clearTextfield() {
  myTextarea.setText("");
}

void updateTextfield(String text) {
  updateTextfield(text, true);
}

void updateTextfield(String text, boolean newline) {
  String s = myTextarea.getText();
  s += text;
  if (newline) {
    s+="\n";
  }
  myTextarea.setText(s);
}

