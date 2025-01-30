#pragma once

#include <functional>
#include <string>
#include "emulator/types.h"
#include "emulator/multimedia/graphicstate.h"

// This structure contains the functions that get called whenever interaction with the underlying
// hardware is needed (for example for button input or timing). They should be as fast as possible,
// as they are executed sequentially with the emulator
typedef struct {
  // This function returns a Byte in which a 1 in bit 0 corresponds to R button pressed, in bit 1 to
  // L, in bit 2 to U, in bit 3 to D, in bit 4 to A, int bit 5 to B, in bit 6 to Sel and in bit 7 to
  // start
  std::function<Byte()> readButtons = nullptr;

  // This function takes a string and prints it (either on console, serial, etc)
  std::function<void(std::string)> print = nullptr;

  // This function takes a string and stores it as a log (in a log file or alike)
  std::function<void(std::string)> logData = nullptr;

  // This function receives a ScreenFrame, which contains the value of each pixel in an intensity
  // scale from 0 to 3, where 0 is white and 3 black, and prints it to screen
  std::function<void(ScreenFrame)> updateScreen = nullptr;
} Interface;