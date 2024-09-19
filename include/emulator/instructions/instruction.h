#pragma once

#include <cstdint>
#include "emulator/state.h"

int instrLen (Byte opcode);

int executeInstruction (Byte opcode, Byte data0, Byte data1, State *state);

int executeCBInstruction (Byte opcode, State *state);