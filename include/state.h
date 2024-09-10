
/*
* This file handles everything related to memory and registers
*/

#pragma once

#include <cstdint>

using Reg = uint8_t;
using Byte = uint8_t;
using DReg = uint16_t;

#define MEM_SIZE 0x10000

// FLEG setting utils
#define SET_ZERO_FLAG(state, value) state.FLEG |= 0x80*(value == 0)
#define SET_SUBSTRACT_FLAG(state) state.FLEG |= 0x40
#define SET_HALF_CARRY_FLAG(state, condition) state.FLEG |= 0x20*condition
#define SET_CARRY_FLAG(state, condition) state.FLEG |= 0x10*condition

// Double register utils
#define JOIN_REGS(r1, r2) (DReg(r1) << 8) | DReg(r2)
#define REG_AF(state) JOIN_REGS(state.A, state.F)
#define REG_BC(state) JOIN_REGS(state.B, state.C)
#define REG_DE(state) JOIN_REGS(state.D, state.E)
#define REG_HL(state) JOIN_REGS(state.H, state.L)


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
  Byte memory[MEM_SIZE] = {0};
} State;