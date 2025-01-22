#pragma once

/*
* This file handles the instruction execution
*/

#include <utility>
#include "emulator/state.h"
#include "emulator/interface.h"

#define CLOCK_FREQ 4194304
#define INSTR_BLOCK_N 10000 // Instructions are executed in groups of X
#define INSTRS_PER_BUTTON_UPDATE 64 // Each X instructions the new buttons pressed check is run


void initializeState (State *state, Interface *interface);

// Execute INSTR_BLOCK_N instructions. If execution flow is interrupted earlier,
// function returns automatically. Returns number of clock cycles executed and
// true if it finished the INSTR_BLOCK_N or false if it terminated from other source.
std::pair<int, bool> executeInstrBlock (State *state, Interface *interface);