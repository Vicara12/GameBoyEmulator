#pragma once

#include <cstdint>
#include "emulator/state.h"


int foo ();

int executeInstruction (Byte opcode, Byte data0, Byte data1, State *state);