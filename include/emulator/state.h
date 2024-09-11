
/*
* This file handles everything related to memory and registers
*/

#pragma once

#include <cstdint>

using Reg = uint8_t;
using Byte = uint8_t;
using SByte = int8_t;
using DReg = uint16_t;
using Short = uint16_t;

#define MEM_SIZE 0x10000

// Flag setting utils
#define ZERO_FLAG       0x80
#define SUBTRACT_FLAG   0x40
#define HALF_CARRY_FLAG 0x20
#define CARRY_FLAG      0x10

#define CLEAR_ALL_FLAGS(state) state->F = 0x00;

#define SET_ZERO_FLAG(state) state->F |= ZERO_FLAG
#define COND_SET_ZERO_FLAG(state, condition) state->F |= ZERO_FLAG*(condition)
#define RESET_ZERO_FLAG(state) state->F &= ~ZERO_FLAG

#define SET_SUBTRACT_FLAG(state) state->F |= SUBTRACT_FLAG
#define RESET_SUBTRACT_FLAG(state) state->F &= ~SUBTRACT_FLAG

#define SET_HALF_CARRY_FLAG(state) state->F |= HALF_CARRY_FLAG
#define COND_SET_HALF_CARRY_FLAG(state, condition) state->F |= HALF_CARRY_FLAG*(condition)
#define RESET_HALF_CARRY_FLAG(state) state->F &= ~HALF_CARRY_FLAG

#define SET_CARRY_FLAG(state) state->F |= CARRY_FLAG
#define COND_SET_CARRY_FLAG(state, condition) state->F |= CARRY_FLAG*(condition)
#define RESET_CARRY_FLAG(state) state->F |= ~CARRY_FLAG

// Double register utils
#define JOIN_REGS(r1, r2) (DReg(r1) << 8) | DReg(r2)
#define REG_AF(state) JOIN_REGS(state->A, state->F)
#define REG_BC(state) JOIN_REGS(state->B, state->C)
#define REG_DE(state) JOIN_REGS(state->D, state->E)
#define REG_HL(state) JOIN_REGS(state->H, state->L)

#define STORE_SHORT(value, dst_high, dst_low) dst_high = Byte(value >> 8); dst_low = Byte(value & 0xFF)
#define SET_REG_AF(value, state) STORE_SHORT(value, state->A, state->F)
#define SET_REG_BC(value, state) STORE_SHORT(value, state->B, state->C)
#define SET_REG_DE(value, state) STORE_SHORT(value, state->D, state->E)
#define SET_REG_HL(value, state) STORE_SHORT(value, state->H, state->L)


typedef struct {
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