/*
StripInvader OSC Interface using Control (http://charlie-roberts.com/Control/)

(c) by michael Vogt/neophob.com 2011
 */

loadedInterfaceName = "stripInvader";
interfaceOrientation = "portrait";



pages = [
/********** PAGE 1 *************/
[

/* RGB slider */
{
    "name":"knbr",
    "type":"Slider",
    "x":0.0, "y":0,
    "width":.2, "height":.45,
    "stroke": "#550000",
    "color": "#ff0000",
    "min": 0.0, "max": 1.0,
    "isXFader" : false,
    "isVertical" : true,
},

{
    "name":"knbg",
    "type":"Knob",
    "x":.33, "y":0,
    "radius":.3,
    "color": "#00ff00",
    "stroke": "#008800",
},
{
    "name":"knbb",
    "type":"Knob",
    "x":.66, "y":0,
    "radius":.3,
    "color": "#0000ff",
    "stroke": "#000088",   
},

/* -- RANDOM PRESET Buttons */
{
     "name": "mode",
     "type": "Button",
     "x":.2, "y":.55,
     "width":.6, "height":.2,
     "mode": "momentary",
     "color": "#fc8000",
     "stroke": "#7e4000",
     "label": "MODE",
     "labelSize": "18",
	 "oninit": "fireRandomPresentButton.fillDiv.style.borderWidth = '2px';",    
},

/* -- Refresh GUI Button */
{
     "name": "refreshButton",
     "type": "Button",
     "x": 0.0, "y": .8,
     "width": .15, "height": .15,
     "mode": "contact",
     "color": "#fc8000",
     "stroke": "#7e4000",
     "ontouchstart": "interfaceManager.refreshInterface()",
     "label": "Refresh GUI",
     "labelSize": "18",
},

],

];



