# StripInvaders

A led strip system controlled by your Smartphone or Tablet. Connect StripInvaders to your LAN and enjoy your own 
Lightshow! Features:

* 15 different Light Modes, more will follow
* Stepless adjust the RGB values​​
* Stepless adjust the animation speed
* Control StripInvaders wireless with your iPhone/iPad/Android Phone/Android Tablet (OSC)
* Bonjour and mDNS support

Website: http://pixelinvaders.ch/?page_id=396

(c) 2011 by Michael Vogt <michu@neophob.com>


## Required Hardware:

* Arduino Ethernet
* ws2801 based led strips, I sell some at [my shop](http://pixelinvaders.ch/?page_id=390)
* 5v regulated power supply


## OSC Messages:

Name:   Type:   Range:   Descr:
/knbr   float   0..1     R Color Vale
/knbg   float   0..1     G Color Vale
/knbb   float   0..1     B Color Vale
/delay  float   0..1     Delay Value
/audio  float   0..1     Enable audio sync (toggle button)
/mode   float   0..1     if mode = 1, switch to next effect
/modd   float   0..n     direct change to effect x
/swp    3x int           first byte must be 66 (magic byte), then datapin and clkpin. this change is 
                         stored in the eeprom. The Arduino will reboot after the update


## Effects:

00: color lines
01: solid color (white)
02: solid color wheel fader
03: solid color random fader
04: color wheel aka rainbow
05: knight rider 1 mover
06: knight rider 4 movers
07: knight rider 8 movers
08: knight rider block
09: xor 1
10: xor 2
11: xor 3
12: xor 4
13: xor 5
14: xor 6


## Network:

  DHCP Enabled (and needed!)
  MAC: 00:00:AF:FE:BE:01
  mDNS: invader.local (ping invader.local)


## TODO:

* Make nr. of LED Pixels configurable via OSC (and do not forget to add a sanity check to prevent out of memory crash)
* more effects, fader fx is not working very good atm.
