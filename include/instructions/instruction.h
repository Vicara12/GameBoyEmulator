#pragma once

#include <cstdint>
#include "state.h"


int executeInstruction (Byte opcode, Byte data0, Byte data1, State& state);