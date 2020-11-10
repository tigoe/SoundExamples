/*
  Web MIDI API example with WebAudioFont player

  Demonstrates the W3C Web MIDI API
  When new Web MIDI devices appear, they are added to an input or output
  select menu. When they disappear, they are removed from the menus.
  Creates a webAudioFont player using this library: https://surikov.github.io/webaudiofont/  
  When you select an input, the input's messages are routed to the webAudioFont player.
  
  This is basically a modification of the WebAudioFont MIDIkey example, modified for p5.js
  and commented in more detail:
  https://github.com/surikov/webaudiofont/blob/master/examples/midikey.html
  
  created 10 Nov 2020
  by Tom Igoe
*/
// select menus for inputs and outputs:
let inputSelect, outputSelect;

// arrays for the MIDI devices:
let inputDevices = new Array();

// variable for the currently selected input:
let currentInput;

// an HTML div for messages:
let messageDiv;

// set up the WebAudioFontPlayer with an acoustic grand piano:
// (see https://surikov.github.io/webaudiofont under "parts of webAudioFont"
// for details of the variable names):
var tone = _tone_0001_FluidR3_GM_sf2_file;
var AudioContextFunc = window.AudioContext || window.webkitAudioContext;
var audioContext = new AudioContextFunc();
var player = new WebAudioFontPlayer();

// an array of notes that are currently playing:
var midiNotes = [];
// unpack the webAudioFont:
player.loader.decodeAfterLoading(audioContext, '_tone_0001_FluidR3_GM_sf2_file');


function setup() {
   noCanvas();
   noLoop();

    // create the message div and position it:
    messageDiv = createDiv('wait for WebAudioFonts to load...');
    messageDiv.position(10, 120);
 
   // create the select menus and position them:
   inputSelect = createSelect();
   inputSelect.position(10, 10);
   inputSelect.changed(selectInput);
   inputSelect.option('--Choose an input:--', 0);

   outputSelect = createSelect();
   outputSelect.position(10, 40);
   outputSelect.changed(selectOutput);
   outputSelect.option('--Choose an output:--', 0);

   // load webAudioFonts:
   loadOutputs();

   // initialize MIDI and get device lists:
   navigator.requestMIDIAccess()
      .then(getDevices);
}

// Get lists of available MIDI controllers
function getDevices(midiAccess) {
   const inputs = midiAccess.inputs.values();

   // add inputs and outputs to the global arrays and the select menus:
   for (let i of inputs) {
      addMidiItem(i);
   }

   // if any of the devices change state, add or delete it:
   midiAccess.onstatechange = function (item) {
      // if an item changes state, add it or delete it from the select menus:
      if (item.port.state == 'connected') {
         addMidiItem(item.port);
      }
      if (item.port.state == 'disconnected') {
         removeMidiItem(item.port);
      }

      // Print information about the changed MIDI controller:
      messageDiv.html(item.port.name + "  "
         + item.port.state);
   };
}

// add new MIDI devices:
function addMidiItem(midiItem) {
   // add to the appropriate select menu,
   // but make sure it's not already in the list:
   if (midiItem.type == 'input' && inputDevices.indexOf(midiItem) < 0) {
      inputSelect.option(midiItem.name);
      // add to the devices array too:
      inputDevices.push(midiItem);
   }

   // add a message listener:
   midiItem.onmidimessage = getMIDIMessage;
}

// remove items when they go away:
function removeMidiItem(midiItem) {
   // choose the right select menu:
   if (midiItem.type == 'input') {
      selectMenu = inputSelect;
      inputDevices.splice(inputDevices.indexOf(midiItem), 1);
   }

   // clear the message listener:
   midiItem.onmidimessage = null;

   // delete the item from the menu:
   for (let i = 0; i < selectMenu.elt.length; i++) {
      if (selectMenu.elt[i].innerHTML === midiItem.name) {
         selectMenu.elt[i].remove();
      }
   }
}

function loadOutputs() {
   // iterate over the instrument list:
   for (var i = 0; i < player.loader.instrumentKeys().length; i++) {
      var optionLabel = '' + (i + 1) + '. ' + player.loader.instrumentInfo(i).title;
      outputSelect.option(optionLabel, i);
   }
}

// select the current input or output from the select menus:
function selectInput() {
   for (let i of inputDevices) {
      if (i.name === inputSelect.selected()) {
         currentInput = i;
      }
   }
   // if they chose the default position, clear the current input:
   if (inputSelect.value() == 0) {
      currentInput = null;
   }
}

function selectOutput() {
   // get the value of the selected item in the outputSelect:
   var chosenOutput = outputSelect.value();
   // load that instrument number:
   var info = player.loader.instrumentInfo(chosenOutput);
   player.loader.startLoad(audioContext, info.url, info.variable);
   // call a listener for when the note is ready:
   player.loader.waitLoad(onLoad);

   // when the note is ready, load its data into the relevant variables:
   function onLoad() {
      tone = window[info.variable];
      player.cancelQueue(audioContext);
   }
}

// MIDI message listener function:
function getMIDIMessage(message) {
   // if the message came from a device other than the current input, you're done:
   if (currentInput !== message.currentTarget) return;

   // MIDI command is the top 4 bits of byte 0:
   var cmd = message.data[0] >> 4;
   // MIDI channel is the bottom 4 bits of byte 0:
   var channel = message.data[0] & 0xf;
   // message type is top 4 bits of byte 0, still in top bits:
   var type = message.data[0] & 0xf0;
   // pitch is byte 1:
   var pitch = message.data[1];
   // velocity is byte 2:
   var velocity = message.data[2];
   switch (type) {
      case 144:  // note on, 0x90
         midNoteOn(pitch, velocity);
         break;
      case 128:  // note off, 0x80
         midiNoteOff(pitch);
         break;
   }
}

function midNoteOn(pitch, velocity) {
   // send a MIDI note off:
   midiNoteOff(pitch);
   // get the envelope for the new note:
   var envelope = player.queueWaveTable(audioContext,
      audioContext.destination,
      tone, 0, pitch, 123456789, velocity / 100);
   // generate the note:
   var note = {
      pitch: pitch,
      envelope: envelope
   };
   // add it to the list of playing notes:
   midiNotes.push(note);
}

function midiNoteOff(pitch) {
   // iterate over the list of currently playing notes:
   for (var i = 0; i < midiNotes.length; i++) {
      // if this note is playing the pitch:
      if (midiNotes[i].pitch == pitch) {
         // and it's still got an envelope in the webAudioFontPlayer:
         if (midiNotes[i].envelope) {
            // stop the envelope playing:
            midiNotes[i].envelope.cancel();
         }
         // remove the note from the list of currently playing notes:
         midiNotes.splice(i, 1);
         return;
      }
   }
}