/*
* Some debugging functionalities
*/

#pragma once

#include "emulator/types.h"
#include "emulator/state.h"
#include "emulator/interface.h"
#include "emulator/instructions/instruction.h"

std::string formatByte (Byte b, bool inc_0x = false);

std::string formatShort (Short s, bool inc_0x = false);

void showRegisters (State *state, Interface *interface);

void showMemoryRange (State *state, Short ini, Short fi, Interface *interface);

State* runInDebug (State *state, Interface *interface);