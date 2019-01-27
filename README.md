# MidiProc
A simple MIDI Processor that coverts stomp switches and expression pedals into MIDI messages.

This is part of a new two-part MIDI control system I'm building for when I play with my band. I currently use a small 1590N1 sized stomp box controller that has several switches and knobs, and a small screen. It was designed to be a good replacement for conventional pedals.

Then one day I had an epiphany. Guitar pedals are designed wrong. My feet are near the floor, my hands are several feet above that! Why should I bend down to turn a knob! Stomp switches below on the floor. Knobs belong at at chest level.

FLOOR - A box containing an inexpensive Teensy LC to process stomp switches and expression pedals and turn them into MIDI signals. Nothing more. I'm using external Boss footswitches so my project box contains TRS jacks and no stomp switches itself.

CHEST - A Teensy 3.2 based design with a 2.8" color touchscreen and a rotary encoder. "Knobs" are shown on the screen. Touch the one you want to change, then turn the rotary encoder physical knob. This will be in a 3D printed case and clamps to a mic stand.

A MIDI cable runs between the floor unit and the chest unit such that all MIDI signals then go from the CHEST unit to the thing you are controlling with MIDI. In my case, the rack audio effects processor.


