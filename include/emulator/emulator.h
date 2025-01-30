/*
* Main file of the emulator
*/

#pragma once

#include "emulator/state.h"
#include "emulator/interface.h"


// Initializes state, loads game into memory and executes boot rom
void boot (State *state, Interface *interface, GameRom *game_rom);

// Executes the game
void run (State *state, Interface *interface);

// Run the emulator (boot + run)
void emulator (Interface *interface, GameRom *game_rom = nullptr);