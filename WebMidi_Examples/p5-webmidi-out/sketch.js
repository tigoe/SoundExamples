/*
  Web MIDI API output example

  Demonstrates the W3C Web MIDI API
  When new Web MIDI devices appear, they are added to an  output
  select menu. When they disappear, they are removed from the menu. 
  When you click in the window, MIDI messages are generated and
  are routed to the output.

  created 10 Nov 2020
  by Tom Igoe
*/
// select menus for inputs and outputs:
let outputSelect;

let currentNotes = new Array();

// arrays for the MIDI devices:
let outputDevices = new Array();

// variables for the currently selected ones:
let currentOutput;

// an HTML div for messages:
let messageDiv;


// an array to hold the final notes of the scale:
let intervals = [2, 2, 1, 2, 2, 2, 1];
let keySig;
// start with middle C:
let tonic = 60;

function setup() {
  createCanvas(windowWidth, windowHeight);
  noStroke();
  outputSelect = createSelect();
  outputSelect.position(10, 40);
  outputSelect.changed(selectOutput);
  outputSelect.option('--Choose an output:--', 0);

  // create the message div and position it:
  messageDiv = createDiv('messages will go here');
  messageDiv.position(10, 120);

  // initialize MIDI and get device lists:
  navigator.requestMIDIAccess()
    .then(getDevices);

    // generate a scale:
    keySig = generateScale(tonic);
}

function draw() {
  background(255);
  for (note of currentNotes) {
    note.fade = note.fade - (1.0 / 127.0);
    note.diameter++;
    if ((note.fade > 0.05) && (note.fade < 1.0)) {
      fill('rgba(23,255,255,' + note.fade + ')');
      circle(note.locH, note.locV, note.diameter);
    } else {
      let midiCmd = [0x80,note.pitch, 0x00];
      sendMidiMessage(midiCmd);
      currentNotes.splice(currentNotes.indexOf(note), 1);
    }
  }
}

// Get lists of available MIDI controllers
function getDevices(midiAccess) {
  const outputs = midiAccess.outputs.values();

  // add   outputs to the global array and the select menu:
  for (let o of outputs) {
    addMidiItem(o);
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
  if (midiItem.type == 'output' && outputDevices.indexOf(midiItem) < 0) {
    outputSelect.option(midiItem.name);
    // add to the devices array too:
    outputDevices.push(midiItem);
  }

  // add a message listener:
  midiItem.onmidimessage = getMIDIMessage;
}

// remove items when they go away:
function removeMidiItem(midiItem) {
  // choose the right select menu:
  if (midiItem.type == 'output') {
    selectMenu = outputSelect;
    outputDevices.splice(outputDevices.indexOf(midiItem), 1);
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


function selectOutput() {
  // iterate over the list of devices:
  for (let o of outputDevices) {
    if (o.name === outputSelect.selected()) {
      currentOutput = o;
    }
  }
  // if they chose the default position, clear the current output:
  if (outputSelect.value() == 0) {
    currentOutput = null;
  }
}

// MIDI message listener function:
function getMIDIMessage(message) {
  // if the message came from a device other than the current input, you're done:
  if (currentInput !== message.currentTarget) return;

  // print the message (print the MIDI bytes as hexadeciimal values):
  messageDiv.html("MIDI message: 0x"
    + message.data[0].toString(16)
    + ", 0x" + message.data[1].toString(16)
    + ", 0x" + message.data[2].toString(16));

  // if the message is intended for the current output, send it there:
  if (currentOutput != null) {
    currentOutput.send(message.data);
  }
}

function mouseReleased() {
  let thisNote = {
    locH: mouseX,
    locV: mouseY,
    diameter: 0,
    fade: 1.0,
    pitch: keySig[floor(8* mouseY / height)]
  }

  let midiCmd = [0x90,thisNote.pitch, 0x64];
  currentNotes.push(thisNote);
  sendMidiMessage(midiCmd);
}

function sendMidiMessage(message) {
  // print the message (print the MIDI bytes as hexadeciimal values):
  messageDiv.html("MIDI message: 0x"
    + message[0].toString(16)
    + ", 0x" + message[1].toString(16)
    + ", 0x" + message[2].toString(16));
  // if the message is intended for the current output, send it there:
  if (currentOutput != null) {
    currentOutput.send(message);
  }
}


function generateScale(tonic) {
  // fill the scale array with the scale you want:
  // start with the initial note:
  let scale = new Array();
  scale.push(tonic);
  let note = tonic + intervals[0];
  // iterate over the intervals, adding each to the next note
  // in the scale:
  for (var n = 0; n < intervals.length; n++) {
    note = note + intervals[n];
    scale.push(note);
  }
  // return the 
  return scale;
}