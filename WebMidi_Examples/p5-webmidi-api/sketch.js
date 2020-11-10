/*
  Web MIDI API example

  Demonstrates the W3C Web MIDI API
  When new Web MIDI devices appear, they are added to an input or output
  select menu. When they disappear, they are removed from the menus. 
  When you select an input and output, the input's messages are routed to the output.
  The slider sends a pitch bend message, using a range from 0-1023 (the range
    of a typical microncontroller ADC).

  created 9 Nov 2020
  by Tom Igoe
*/
// select menus for inputs and outputs:
let inputSelect, outputSelect;

// arrays for the MIDI devices:
let outputDevices = new Array();
let inputDevices = new Array();

// variables for the currently selected ones:
let currentOutput, currentInput;

// an HTML div for messages:
let messageDiv;
// HTML slider to send a pitch bend message:
let pitchBender;

function setup() {
  noCanvas();
  noLoop();

  // create the select menus and position them:
  inputSelect = createSelect();
  inputSelect.position(10, 10);
  inputSelect.changed(selectInput);
  inputSelect.option('--Choose an input:--', 0);

  outputSelect = createSelect();
  outputSelect.position(10, 40);
  outputSelect.changed(selectOutput);
  outputSelect.option('--Choose an output:--', 0);

  // set up the pitch bend slider:
  pitchBender = createSlider(0, 1023, 512);
  pitchBender.position(10, 80);
  pitchBender.changed(sendPitchBend);
  let pitchBendLabel = createSpan('Pitch Bend');
  pitchBendLabel.position(180, 80);

  // create the message div and position it:
  messageDiv = createDiv('messages will go here');
  messageDiv.position(10, 120);

  // initialize MIDI and get device lists:
  navigator.requestMIDIAccess()
    .then(getDevices);
}

// Get lists of available MIDI controllers
function getDevices(midiAccess) {
  const inputs = midiAccess.inputs.values();
  const outputs = midiAccess.outputs.values();

  // add inputs and outputs to the global arrays and the select menus:
  for (let i of inputs) {
    addMidiItem(i);
  }
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
  if (midiItem.type == 'input' && inputDevices.indexOf(midiItem) < 0) {
    inputSelect.option(midiItem.name);
    // add to the devices array too:
    inputDevices.push(midiItem);
  }
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
  if (midiItem.type == 'input') {
    selectMenu = inputSelect;
    inputDevices.splice(inputDevices.indexOf(midiItem), 1);
  }
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

// if the mouse is pressed on the pitch bend slider, make a pitch bend:
function mouseDragged() {
  sendPitchBend();
}

function sendPitchBend() {
  let pitchBend = pitchBender.value();
  var msb = floor((pitchBend << 1) / 256);      // get the high 7 bits
  var lsb = floor(pitchBend % 128);  // get the low 7 bits
  // pitch bend:
  var message = [0xE0, lsb, msb];

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