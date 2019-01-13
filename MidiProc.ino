#include <MIDI.h>

#include "BAPhysicalControls.h"
#include "Pinout.h"
#include "MidiMapping.h"
#include "ExpressionCalib.h"

#define USE_MIDI // comment out to use basic serial instead of compliant MIDI

#ifdef USE_MIDI
MIDI_CREATE_DEFAULT_INSTANCE();
using namespace midi;
#endif

using namespace BALibrary;

// To get the calibration values for your particular board, first run the
// BAExpansionCalibrate.ino example and 
int  potCalibMin = 319;
int  potCalibMax = 955;
bool potSwapDirection = false;

// Create a control object using the number of switches, pots, encoders and outputs on the
// Blackaddr Audio Expansion Board.
BAPhysicalControls controls(NUM_SWITCHES, NUM_POTS, 0, 0);

// BAPhysicalControls returns a handle when you register a new control. We'll uses these handles when working with the controls.
int sw0Ring, sw0Tip, sw1Ring, sw1Tip, expTip, expRing; // Handles for the various controls

constexpr int SW_INDEX_START  = 0;
constexpr int SW_INDEX_END    = SW_INDEX_START + NUM_SWITCHES-1;
constexpr int EXP_INDEX_START = SW_INDEX_END + 1;
constexpr int EXP_INDEX_END   = EXP_INDEX_START + NUM_POTS -1;
int midiMap[6][2]; // Create an array to map handles to MIDI

int teensyLed = 0;
unsigned loopCounter = 0;
bool testVal = 0;

DataByte dummy;

void setup() {
  // put your setup code here, to run once:

  delay(100);
  Serial.begin(115200);
  delay(100);
  while (!Serial) {}
  Serial.println("Starting Config");

  #ifdef USE_MIDI
    MIDI.begin(MIDI_CHANNEL_OMNI);
  #else
    Serial1.begin(SERIAL_BAUD_RATE, SERIAL_8N1);
  #endif
  
  pinMode(EXP_TIP_ANALOG, INPUT);
  pinMode(EXP_RING_ANALOG, INPUT);
  pinMode(SW0_RING, INPUT);
  pinMode(SW0_TIP,  INPUT);
  pinMode(SW1_RING, INPUT);
  pinMode(SW1_TIP,  INPUT);
  pinMode(TEENSY_LED, OUTPUT);
  //pinMode(TXD0, OUTPUT);

  // Setup the physical controls
  sw0Tip  = controls.addSwitch(SW0_TIP);
  sw0Ring = controls.addSwitch(SW0_RING);
  sw1Tip  = controls.addSwitch(SW1_TIP);
  sw1Ring = controls.addSwitch(SW1_RING);
  expTip  = controls.addPot(EXP_TIP, potCalibMin, potCalibMax, potSwapDirection);
  expRing = controls.addPot(EXP_RING, potCalibMin, potCalibMax, potSwapDirection);

  midiMap[0][0] = sw0Tip;
  midiMap[0][1] = SW0_TIP_CC;
  midiMap[1][0] = sw0Ring;
  midiMap[1][1] = SW0_RING_CC;
  midiMap[2][0] = sw1Tip;
  midiMap[2][1] = SW1_TIP_CC;
  midiMap[3][0] = sw1Ring;
  midiMap[3][1] = SW1_RING_CC;
  midiMap[4][0] = expTip;
  midiMap[4][1] = EXP_TIP_CC;
  midiMap[5][0] = expRing;
  midiMap[5][1] = EXP_RING_CC;

  Serial.println("Done Config");

  // Check for calibration
  if (controls.isSwitchHeld(sw0Tip)) {
    // switch is held, enter calibration mode once release
    if (Serial) { Serial.println("Entering calibration mode, please release SW0"); }
    while (controls.isSwitchHeld(sw0Tip)) {}
    
    expressionCalib(potCalibMin, potCalibMax, potSwapDirection, controls, sw0Tip, expTip);
    controls.setCalibrationValues(expTip, potCalibMin, potCalibMax, potSwapDirection);
  }

}

void sendData(DataByte midiCC, DataByte midiValue, DataByte midiChannel)
{
  #ifdef USE_MIDI
    MIDI.sendControlChange(midiCC, midiValue, midiChannel);
  #else
    Serial1.write(midiCC);
    Serial1.write(midiValue);
    Serial1.write(midiChannel);
    Serial1.flush();
  #endif
  if (Serial) { Serial.println(String("Send MIDI CC=") + midiCC + String(" VAL=") + midiValue + String(" CH=") + midiChannel); }
}


void loop() {

  //while(true) { delay(10); }
  
  float potValue;
  bool  switchValue;
  

  for (int i=SW_INDEX_START; i<=SW_INDEX_END; i++) {
    if (controls.hasSwitchChanged(midiMap[i][0], switchValue)) {
      //if (Serial) { Serial.println(String("SWITCH ") + i + String(" CHANGED, set to ") + switchValue); }
      DataByte midiCC = midiMap[i][1];
      // if switch value is positive send MIDI ON, else MIDI OFF
      DataByte midiValue = switchValue ? MIDI_ON_VALUE : MIDI_OFF_VALUE;
      sendData(midiCC, midiValue, MIDI_SEND_CHANNEL);      
    }
  }

  for (int i=EXP_INDEX_START; i<=EXP_INDEX_END; i++) {
    if (controls.checkPotValue(midiMap[i][0], potValue)) {
      // Pot has changed
      //if (Serial) { Serial.println(String("POT Setting for ") + i + String(", ") + potValue); }
      DataByte midiCC = midiMap[i][1];
      DataByte midiValue = 0; // set a default value

      // It's possible if calibration values aren't perfect, pot values less than 0.0 or greater than 1.0 can be returned.
      if (potValue > 0.0f) {
        // Check if pot value is in range, if so map from 0 to 127, otherwise clip to 127
        if (potValue <= 1.0f) {     
          midiValue = static_cast<DataByte>(127.0f * potValue);
        } else {
          midiValue = 127;
        }
      }
      sendData(midiCC, midiValue, MIDI_SEND_CHANNEL);
    }
  }

  delay(10);

  // Blink the LED about 2 per second
  if ((loopCounter % 25) == 0) {
    digitalWrite(TEENSY_LED, teensyLed);
    teensyLed = ~teensyLed;
  }

//  if ((loopCounter %200) == 0) {
//    digitalWrite(TXD0, testVal & 0x1);
//    testVal = testVal ? 0 : 1;
//    Serial.println(String("TXD0 is now ") + (testVal & 0x1));
//  }

  loopCounter++;

}
