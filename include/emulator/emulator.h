/*
* Main file of the emulator
*/

#pragma once

#include "emulator/state.h"
#include "emulator/interface.h"



// Run the emulator (boot + run)
void emulator (Interface *interface, GameRom *game_rom = nullptr);