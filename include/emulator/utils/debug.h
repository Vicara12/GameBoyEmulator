/*
* Some debugging functionalities
*/

#pragma once

#include "emulator/types.h"
#include "emulator/state.h"
#include "emulator/interface.h"

std::string formatByte (Byte b);

std::string formatShort (Short s);

void showRegisters (State *state, Interface *interface);

void showMemoryRange (State *state, Short ini, Short fi, Interface *interface);