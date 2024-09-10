
/*
* This file handles everything related to memory and registers
*/

#pragma once

#include <cstdint>

#define SET_ZERO_FLAG(state, value) state.FLEG |= 0x80*(value == 0)
#define SET_SUBSTRACT_FLAG(state) state.FLEG |= 0x40
#define SET_HALF_CARRY_FLAG(state, condition) state.FLEG |= 0x20*condition
#define SET_CARRY_FLAG(state, condition) state.FLEG |= 0x10*condition

using Reg = uint8_t;
using Byte = uint8_t;
using DReg = uint16_t;


typedef struct {
  Reg FLEG = 0;
  Reg A = 0;
  Reg B = 0;
  Reg C = 0;
  Reg D = 0;
  Reg E = 0;
  Reg F = 0;
  Reg H = 0;
  Reg L = 0;
  DReg SP = 0xFFFE;
  DReg PC = 0x0100;
} State;