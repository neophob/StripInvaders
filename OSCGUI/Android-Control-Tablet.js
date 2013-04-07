/*
StripInvader OSC Interface using Control (http://charlie-roberts.com/Control/)

(c) original by michael Vogt/neophob.com 2011 (https://github.com/neophob/StripInvaders/blob/master/OSCGUI/si.js)
(c) edit by Ruben VC 2013-04-07 WIP
 */
loadedInterfaceName = "KnightBench";
interfaceOrientation = "portrait";

pages = [
/********** PAGE 1 *************/
[
/* Titel */
{
    "name": "infoText",
    "type": "Label",
    "x": 0, "y": 0.01,
    "width": 1, "height": 1,
    "value": "THE KNIGHT BENCH CONTROL PANEL",
    "verticalCenter": false,
},
/* RGB slider */
{
    "name":"knbr",
    "type":"Slider",
    "x":0.01, "y":0.03,
    "width":.22, "height":.35,
    "stroke": "#ff0000",
    "color": "#ff0000",
    "min": 0.0, "max": 1.0,
    "isXFader" : false,
    "isVertical" : true,
    "ontouchmove" : "var number = this.value*255; var n = number.toFixed(); knbrValue.changeValue(n)",
},
{
     "name" : "knbroff",
     "type" : "Button",
     "x" : 0.01, "y" : 0.39,
     "width" : .05, "height" : .03,
     "color": "#ffffff",
     "stroke": "#ff0000",
     "labelSize": "18",
     "label": "0",
     "mode": "contact",
     "min": 0.0, "max": 0.07,
     "ontouchstart": "oscManager.sendOSC(['/knbr', 'f', 0.001]); var number = 0; var n = number.toFixed(); knbrValue.changeValue(n); knbr.setValue(n)",
},
{
     "name" : "knbronn",
     "type" : "Button",
     "x" : 0.18, "y" : 0.39,
     "width" : .05, "height" : .03,
     "color": "#ffffff",
     "stroke": "#ff0000",
     "labelSize": "18",
     "label": "255",
     "mode": "contact",
     "ontouchstart": "var number = 255; var n = number.toFixed(); knbrValue.changeValue(n); knbr.setValue(1); oscManager.sendOSC(['/knbr', 'f', 1.001]); ",
},
{
    "name": "knbrLabel",
    "type": "Label",
    "x": 0.01, "y": 0.375,
    "width": .22, "height": .05,
    "color": "#ffffff",
    "value": "R",
},
{
    "name": "knbrValue",
    "type": "Label",
    "x": 0.01, "y": 0.385,
    "width": .22, "height": .05,
    "color": "#ffffff",
    "value": "0",
},
{
    "name":"knbg",
    "type":"Slider",
    "x":0.26, "y":0.03,
    "width":.22, "height":.35,
    "stroke": "#00ff00",
    "color": "#00ff00",
    "min": 0.0, "max": 1.0,
    "isXFader" : false,
    "isVertical" : true,
    "ontouchmove" : "var number = this.value*255; var n = number.toFixed(); knbgValue.changeValue(n)",
},
{
     "name" : "knbgoff",
     "type" : "Button",
     "x" : 0.26, "y" : 0.39,
     "width" : .05, "height" : .03,
     "color": "#ffffff",
     "stroke": "#00ff00",
     "labelSize": "18",
     "label": "0",
     "mode": "contact",
     "min": 0.0, "max": 0.07,
     "ontouchstart": "oscManager.sendOSC(['/knbg', 'f', 0.001]); var number = 0; var n = number.toFixed(); knbgValue.changeValue(n); knbg.setValue(n)",
},
{
     "name" : "knbronn",
     "type" : "Button",
     "x" : 0.43, "y" : 0.39,
     "width" : .05, "height" : .03,
     "color": "#ffffff",
     "stroke": "#00ff00",
     "labelSize": "18",
     "label": "255",
     "mode": "contact",
     "ontouchstart": "var number = 255; var n = number.toFixed(); knbgValue.changeValue(n); knbg.setValue(1); oscManager.sendOSC(['/knbg', 'f', 1.001])",
},
{
    "name": "knbgLabel",
    "type": "Label",
    "x": 0.26, "y": 0.375,
    "width": .22, "height": .05,
    "color": "#ffffff",
    "value": "G",
},
{
    "name": "knbgValue",
    "type": "Label",
    "x": 0.26, "y": 0.385,
    "width": .22, "height": .05,
    "color": "#ffffff",
    "value": "0",
},
{
    "name":"knbb",
    "type":"Slider",
    "x":0.51, "y":0.03,
    "width":.22, "height":.35,
    "stroke": "#0000ff",
    "color": "#0000ff",
    "min": 0.0, "max": 1.0,
    "isXFader" : false,
    "isVertical" : true,
    "ontouchmove" : "var number = this.value*255; var n = number.toFixed(); knbbValue.changeValue(n)",

},
{
     "name" : "knbboff",
     "type" : "Button",
     "x" : 0.51, "y" : 0.39,
     "width" : .05, "height" : .03,
     "color": "#ffffff",
     "stroke": "#0000ff",
     "labelSize": "18",
     "label": "0",
     "mode": "contact",
     "min": 0.0, "max": 0.07,
     "ontouchstart": "oscManager.sendOSC(['/knbb', 'f', 0.001]); var number = 0; var n = number.toFixed(); knbbValue.changeValue(n); knbb.setValue(n)",
},
{
     "name" : "knbbonn",
     "type" : "Button",
     "x" : 0.68, "y" : 0.39,
     "width" : .05, "height" : .03,
     "color": "#ffffff",
     "stroke": "#0000ff",
     "labelSize": "18",
     "label": "255",
     "mode": "contact",
     "ontouchstart": "var number = 255; var n = number.toFixed(); knbbValue.changeValue(n); knbb.setValue(1); oscManager.sendOSC(['/knbb', 'f', 1.001])",
},
{
    "name": "knbbLabel",
    "type": "Label",
    "x": 0.51, "y": 0.375,
    "width": .22, "height": .05,
    "color": "#ffffff",
    "value": "B",
},
{
    "name": "knbbValue",
    "type": "Label",
    "x": 0.51, "y": 0.385,
    "width": .22, "height": .05,
    "color": "#ffffff",
    "value": "0",
},

/* speed slider */
{
    "name":"delay",
    "type":"Slider",
    "x":0.76, "y":0.03,
    "width":.22, "height":.25,
    "stroke": "#ffffff",
    "color": "#ffffff",
    "min": 0.0, "max": 1.0,
    "isXFader" : false,
    "isVertical" : true,
},
{
    "name": "delayLabel",
    "type": "Label",
    "x": 0.76, "y": 0.245,
    "width": .22, "height": .05,
    "color": "#ff0000",
    "value": "Delay",
},
//* favorite color */
{
     "name" : "favcolor",
     "type" : "Button",
     "x" : 0.76, "y" : 0.295,
     "width" : .22, "height" : .085,
     "color": "#FF4C00",
     "stroke": "#FF4C00",
     "labelSize": "18",
     "label": "Cosy",
     "mode": "contact",
     "ontouchstart": "knbbValue.changeValue(0); knbrValue.changeValue(255); knbgValue.changeValue(0.300*255); knbr.setValue(1); knbg.setValue(0.300); knbb.setValue(0); oscManager.sendOSC(['/modd', 'f', 1.007]); oscManager.sendOSC(['/knbr', 'f', 1.001]); oscManager.sendOSC(['/knbg', 'f', 0.301]); oscManager.sendOSC(['/knbb', 'f', 0.001])",
},
/* MODE Buttons*/
{
     "name" : "modd0",
     "type" : "Button",
     "x" : 0.01, "y" : 0.45,
     "width" : .17, "height" : .1,
     "color": "#ff0000",
     "stroke": "#ffffff",
     "label": "Lines",
     "labelSize": "18",
     "min": 0.07, "max": 0.07,
     "address": "/modd",
     "mode": "contact",
},
{
     "name" : "modd1",
     "type" : "Button",
     "x" : 0.21, "y" : 0.45,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Solid Color",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 1.07,
     "address": "/modd",
}, 
{
     "name" : "modd2",
     "type" : "Button",
     "x" : 0.41, "y" : 0.45,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Solid Fader",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 2.07,
     "address": "/modd",
},
{
     "name" : "modd3",
     "type" : "Button",
     "x" : 0.61, "y" : 0.45,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Random Fader",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 3.07,
     "address": "/modd",
},
{
     "name" : "modd4",
     "type" : "Button",
     "x" : 0.81, "y" : 0.45,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Rainbow",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 4.07,
     "address": "/modd",
},
{
     "name" : "modd5",
     "type" : "Button",
     "x" : 0.01, "y" : 0.56,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Knight Rider 1",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 5.07,
     "address": "/modd",
},
{
     "name" : "modd6",
     "type" : "Button",
     "x" : 0.21, "y" : 0.56,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Knight Rider 4",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 6.07,
     "address": "/modd",
},
{
     "name" : "modd7",
     "type" : "Button",
     "x" : 0.41, "y" : 0.56,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Knight Rider 8",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 7.07,
     "address": "/modd",
},
{
     "name" : "modd8",
     "type" : "Button",
     "x" : 0.61, "y" : 0.56,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Fader 1",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 8.07,
     "address": "/modd",
},
{
     "name" : "modd9",
     "type" : "Button",
     "x" : 0.81, "y" : 0.56,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Fader 2",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 9.07,
     "address": "/modd",
},
{
     "name" : "modd10",
     "type" : "Button",
     "x" : 0.01, "y" : 0.67,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Fader 3",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 10.07,
     "address": "/modd",
},
{
     "name" : "modd11",
     "type" : "Button",
     "x" : 0.21, "y" : 0.67,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Fader 4",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 11.07,
     "address": "/modd",
},
{
     "name" : "modd12",
     "type" : "Button",
     "x" : 0.41, "y" : 0.67,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Fader 5",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 12.07,
     "address": "/modd",
},
{
     "name" : "modd13",
     "type" : "Button",
     "x" : 0.61, "y" : 0.67,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Fader 5",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 13.07,
     "address": "/modd",
},
{
     "name" : "modd14",
     "type" : "Button",
     "x" : 0.81, "y" : 0.67,
     "width" : .17, "height" : .1,
     "color": "#ffffff",
     "stroke": "#ffffff",
     "label": "Fader 6",
     "labelSize": "18",
     "mode": "contact",
     "min": 0.07, "max": 14.07,
     "address": "/modd",
},

/* -- Refresh GUI Button */
{
     "name": "refreshButton",
     "type": "Button",
     "x": 0.51, "y": .8,
     "width": .22, "height": .1,
     "mode": "contact",
     "color": "#ffffff",
     "stroke": "#ff0000",
     "ontouchstart": "interfaceManager.refreshInterface()",
     "label": "Update",
     "labelSize": "18",
},
/* -- Button for enabling Menu/Toolbar */
{
    "name": "tabButton",
    "type": "Button",
    "x": 0.76, "y": .8,
    "width": .22, "height": .1,
    "mode": "toggle",
    "color": "#ffffff",
    "stroke": "#ff0000",
    "isLocal": true,
    "ontouchstart": "if(this.value == this.max) { control.showToolbar(); } else { control.hideToolbar(); }",
    "label": "Menu",
    "labelSize": "18",
},
/* -- Save Preset */
{
     "name": "sav",
     "type": "Button",
     "x": 0.01, "y": .8,
     "width": .22, "height": .1,
     "mode": "contact",
     "color": "#ffffff",
     "stroke": "#ff0000",
     "label": "Preset",
     "labelSize": "18",
},

/* -- RANDOM PRESET Buttons */
{
     "name": "mode",
     "type": "Button",
     "x": 0.26, "y":.8,
     "width":.22, "height":.1,
     "mode": "momentary",
     "min":0.0, "max":1.0,
     "color": "#ffffff",
     "stroke": "#ff0000",
     "label": "Mode",
     "labelSize": "18",
},

],

];


