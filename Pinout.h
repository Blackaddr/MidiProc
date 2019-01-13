#ifndef _INC_PINOUT_H
#define _INC_PINOUT_H

constexpr int SERIAL_BAUD_RATE = 115200;

// Data comms
constexpr int TXD0 = 1;
constexpr int TXD1 = 4;
constexpr int RXD1 = 3;

// Expression pedal
constexpr int NUM_POTS = 2;
constexpr int EXP_TIP_ANALOG  = A1;
constexpr int EXP_TIP = 15;
constexpr int EXP_RING_ANALOG = A4;
constexpr int EXP_RING = 18;

// Stomp switches
constexpr int NUM_SWITCHES = 4;
constexpr int SW0_RING = 20;
constexpr int SW0_TIP  = 19;
constexpr int SW1_RING = 22;
constexpr int SW1_TIP  = 21;

constexpr int TEENSY_LED = 13;

#endif
