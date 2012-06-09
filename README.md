# StripInvaders

A led strip system controlled by your Smartphone or Tablet. Connect StripInvaders to your LAN and enjoy your own 
Lightshow! 

Features:

* 15 different Light Modes, more will follow
* Stepless adjust the RGB values​​
* Stepless adjust the animation speed
* Control StripInvaders wireless with your iPhone/iPad/Android Phone/Android Tablet (OSC)
* Bonjour and mDNS support
* DHCP support
* Configure your strip (connected pins, nr. of leds) without reflashing firmware, use OSC to configure

**Website:** http://pixelinvaders.ch/?page_id=396

(c) 2011 by Michael Vogt <michu@neophob.com>


## Required Hardware:

* Arduino Ethernet
* ws2801 based led strips, I sell some at [my shop](http://pixelinvaders.ch/?page_id=390)
* 5v regulated power supply
* Optional: Seeedstudio [Grove Base Shield](http://www.seeedstudio.com/depot/grove-base-shield-p-754.html?cPath=132_134)
* Optional: Seeedstudio [Grove - Universal 4 Pin Cable](http://www.seeedstudio.com/depot/grove-universal-4-pin-20cm-cable-5-pcs-pack-p-749.html?cPath=178_179)
* If you want to use the Grove Base Shield, make sure you need 2x 8-pin Header and 2x 6-pin Header, like the [Seeeduino Mega Protoshield Kit](http://www.seeedstudio.com/depot/seeeduino-mega-protoshield-kit-p-597.html?cPath=132_134)

## OSC Messages:

    Name:   Type:   Range:   Descr:
    /knbr   float   0..1     R Color Vale
    /knbg   float   0..1     G Color Vale
    /knbb   float   0..1     B Color Vale
    /delay  float   0..1     Delay Value
    /audio  float   0..1     Enable audio sync (toggle button)
    /mode   float   0..1     if mode = 1, switch to next effect
    /modd   float   0..n     direct change to effect x
    /cfg    4x int           first byte must be 66 (magic byte), then DATAPIN, CLKPIN and then LEDCOUNT. this change is 
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

* More effects (Plasma, Fire, Perlin Noise, FFT)
* Improve Audiosync
* Reduce OSC Library size
* Gamma correction
* Test if OSC Workarround is still needed with Arduino v1.0.1


## Misc:
* One Firmware for both drivers (WS2801/LPD8806) is not possible, due size limitation.
* check http://neophob.com/2012/01/stripinvaders-howto-assemble/ for more information how to assemble the LED Strip

## OSC GUI
StripInvaders provide some ready to use OSC GUI's:

* [OSX/WIN](https://github.com/neophob/StripInvaders/downloads) Application to configure the PixelController
* [PureData](http://puredata.info/) OSC Gui, path: OSCGUI/simple.pd (needed to configure the PixelController)
* [Control](http://charlie-roberts.com/Control/) OSC Gui, path: OSCGUI/si.js
* [TouchOSC](http://hexler.net/software/touchosc) OSC Gui, path: OSCGUI/StripInvaders_iPhone.touchosc
* [TouchOSC](http://hexler.net/software/touchosc) OSC Gui, path: OSCGUI/StripInvaders_iPad.touchosc

## Development Hints

The main issue is the small ROM on arduino (<32kb) when using all libraries.

### EthernetBonjour
* Also due to similar memory limitations, you can only vend up to 8 services running on your board. If you want to change this limitation, edit "NumMDNSServiceRecords" in EthernetBonjour.h (but note that the WIZnet chipset on the ethernet shield only supports 4 sockets anyway, one already taken up by the Bonjour library, so 8 services might be way too much anyway. Thus, you could also lower this constant to free up some memory).

-> I use only one service: `#define  NumMDNSServiceRecords   (1)`
