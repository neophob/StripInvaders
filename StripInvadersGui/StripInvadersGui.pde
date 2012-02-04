import controlP5.*;
import oscP5.*;
import netP5.*;

private static final String OSC_MESSAGE = "/cfg";
private static final int OSC_MAGIC = 66;
private static final int OSC_PORT = 10000;

private static final String HOSTNAME = "invader.local";

//gui
private ControlP5 cp5;
private Textarea myTextarea;
private Button btnPing, btnConfig, btnDefault;
private Slider sData, sClk, sCount;

//control
private NetAddress stripInvader;

private int frames;
private int dataPin;
private int clockPin;
private int ledCount;

void setup() {
  size(600, 350);
  background(0);
  frameRate(25);
  smooth();

  doStripInvaderDefaultConfig();
  initGui();
}



void draw() {
  background(0);

  if (frames==1) {
    ping();
  } 
  else if (frames==0) {
    updateTextfield("Please wait, searching StripInvader Controller...");
  }

  frames++;
}

