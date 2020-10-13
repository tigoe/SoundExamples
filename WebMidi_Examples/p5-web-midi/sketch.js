/*
   WebMIDI and p5.js example

   This example shows how to play webMIDI notes using p5.js

   created 12 Oct 2020
   by Tom Igoe
*/

// variables to hold the midiOutput and midiIput MIDI objects
let midiOutput = false;
let midiIput = false;

let bpm = 72;  // beats per minute
// duration of a beat in ms
let beatDuration = 60.0 / bpm * 1000;

// the melody sequence (Steve Reich's "Piano Phase"):
let melody = new Array(64, 66, 71, 73, 74, 66, 64, 73, 71, 66, 74, 73);
// which note of the melody to play:
let noteCounter = 0;

// option select objects, for picking ins and outs:
let outs;
let ins;

function setup() {
   // check to see if web MIDI is enabled or not:
   WebMidi.enable(midiCheck);
}

function draw() {
   // if there's a selected output:
   if (midiOutput) {
      // play a note from the melody:
      midiOutput.playNote(melody[noteCounter]);
      let noteOnTime = millis();
      // all the notes in this are sixteenth notes,
      // which is 1/4 of a beat, so:
      let noteDuration = beatDuration / 4;
      // stop for the appropriate duration:
      while (millis() - noteOnTime < noteDuration);
      // turn the note off:
      midiOutput.stopNote(melody[noteCounter]);
      // increment the note number for next time through the loop:
      noteCounter++;
      // keep the note in the range from 0 - 11 using modulo:
      noteCounter = noteCounter % 12;
   }
}

// check for MIDI functionality:
function midiCheck(error) {
   // if the call to enable MIDI fails, notify the user:
   if (error) {
      alert("WebMidi could not be enabled.", error);
      // if MIDI's enabled, list the inputs and outputs:
   } else {
      // create select menus for the inputs and outputs:
      listMidiInputs();
      listMidiOutputs();
   }
}


function listMidiInputs() {
   // create the input select, and position it:
   ins = createSelect();
   ins.position(10, 10);
   // set the first option item and select it as the default:
   ins.option('--select input--');
   ins.selected('--select input--');
   // iterate over the MIDI inputs to get the names of the other items:
   for (let i = 0; i < WebMidi.inputs.length; i++) {
      ins.option(WebMidi.inputs[i].name);
   }
   // set a handler for when the user changes the input:
   ins.changed(setMidiInput);
}

function setMidiInput() {
   // get the selected output:
   midiIput = WebMidi.getInputByName(ins.value());
}

function listMidiOutputs() {
   // create the output select, and position it:
   outs = createSelect();

   outs.position(150, 10);
   // set the first option item and select it as the default:
   outs.option('--select output--');
   outs.selected('--select output--');
   // iterate over the MIDI outputs to get the names of the other items:
   for (let i = 0; i < WebMidi.outputs.length; i++) {
      outs.option(WebMidi.outputs[i].name);
   }
   // set a handler for when the user changes the output:
   outs.changed(setMidiOutput);
}

function setMidiOutput() {
   // get the selected output:
   midiOutput = WebMidi.getOutputByName(outs.value());
}
