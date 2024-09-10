#pragma once

#include <cstdint>
#include "state.h"

int executeInstruction (Byte opcode, Byte data, State& state);