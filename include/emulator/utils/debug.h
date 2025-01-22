#pragma once

#include "emulator/state.h"
#include <Arduino.h>

String formatByte (Byte b);

String formatShort (Short s);

void showRegisters (State *state);

void showMemoryRange (State *state, Short ini, Short fi);