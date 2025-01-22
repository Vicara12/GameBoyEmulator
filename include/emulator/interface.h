#pragma once

#include <functional>
#include "emulator/types.h"

// This structure contains the functions that get called whenever interaction with the underlying
// hardware is needed (for example for button input or timing). They should be as fast as possible,
// as they are executed sequentially with the emulator
typedef struct {
  // This function takes a boolean which indicates wether the target button set to read is the
  // R/L/U/D (if true) or A/B/Sel/Start (if false) buttons and returns a Byte in which a 1 in bit 0
  // corresponds to R/A buttons, bit 1 to L/B, bit 2 to U/Sel and bit 3 to D/Start.
  std::function<Byte(bool)> readButtons = nullptr;

  // This function takes a string and prints it (either on console, serial, etc)
  std::function<void(std::string)> print = nullptr;

  // This function takes a string and stores it as a log (in a log file or alike)
  std::function<void(std::string)> logData = nullptr;
} Interface;