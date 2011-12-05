/*
StripInvader OSC Interface using Control (http://charlie-roberts.com/Control/)

(c) by michael Vogt/neophob.com 2011
 */

loadedInterfaceName = "stripInvaderBeta";
interfaceOrientation = "portrait";



pages = [
/********** PAGE 1 *************/
[

/* RGB slider */
{
    "name":"knbr",
    "type":"Slider",
    "x":0.0, "y":0.1,
    "width":.22, "height":.4,
    "stroke": "#550000",
    "color": "#ff0000",
    "min": 0.0, "max": 1.0,
    "isXFader" : false,
    "isVertical" : true,
},
{
    "name":"knbg",
    "type":"Slider",
    "x":0.25, "y":0.1,
    "width":.22, "height":.4,
    "stroke": "#008800",
    "color": "#00ff00",
    "min": 0.0, "max": 1.0,
    "isXFader" : false,
    "isVertical" : true,
},
{
    "name":"knbb",
    "type":"Slider",
    "x":0.5, "y":0.1,
    "width":.22, "height":.4,
    "stroke": "#000088",
    "color": "#0000ff",
    "min": 0.0, "max": 1.0,
    "isXFader" : false,
    "isVertical" : true,
},
{
    "name":"delay",
    "type":"Slider",
    "x":0.75, "y":0.1,
    "width":.22, "height":.4,
    "stroke": "#888888",
    "color": "#ffffff",
    "min": 0.0, "max": 1.0,
    "isXFader" : false,
    "isVertical" : true,
},

/* -- RANDOM PRESET Buttons */
{
     "name": "mode",
     "type": "Button",
     "x":.0, "y":.55,
     "width":.6, "height":.2,
     "mode": "momentary",
     "color": "#fc8000",
     "stroke": "#7e4000",
     "label": "MODE",
     "labelSize": "18",
},
/* -- AUDIO toggle PRESET Buttons */
{
     "name": "audio",
     "type": "Button",
     "x":.6, "y":.55,
     "width":.35, "height":.2,
     "mode": "toggle",
     "color": "#fc8000",
     "stroke": "#7e4000",
     "label": "AUDIO",
     "labelSize": "18",
},

/* -- Refresh GUI Button */
{
     "name": "refreshButton",
     "type": "Button",
     "x": 0.5, "y": .8,
     "width": .25, "height": .1,
     "mode": "contact",
     "color": "#fc8000",
     "stroke": "#7e4000",
     "ontouchstart": "interfaceManager.refreshInterface()",
     "label": "Update",
     "labelSize": "18",
},
/* -- Button for enabling Menu/Toolbar */
{
    "name": "tabButton",
    "type": "Button",
    "x": 0.8, "y": .8,
    "width": .25, "height": .1,
    "mode": "toggle",
    "color": "#fc8000",
    "stroke": "#7e4000",
    "isLocal": true,
    "ontouchstart": "if(this.value == this.max) { control.showToolbar(); } else { control.hideToolbar(); }",
    "label": "Menu",
    "labelSize": "18",
},
],

];



