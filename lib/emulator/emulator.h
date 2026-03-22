/*
* Main file of the emulator
*/

#pragma once

#include "types.h"
#include "state.h"
#include "interface.h"



// Run the emulator (boot + run)
void emulator (Interface *interface, GameRom *game_rom = nullptr, bool debug = false);