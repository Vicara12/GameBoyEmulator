#pragma once

/*
* Code for all the bit arithmetic instructions
*/

#include "emulator/state.h"
#include "emulator/interface.h"

// SWAP n: swap upper and lower nibbles of n
inline int instr_SWAP_n (Reg &n, State *state)
{
  CLEAR_ALL_FLAGS(state);
  COND_SET_ZERO_FLAG(state, n == 0);
  Byte low_nibble = n & 0x0F;
  n = (n >> 4) | (low_nibble << 4);
  return 8;
}

// SWAP (HL): swap upper and lower nibbles of data stored at (HL)
inline int instr_SWAP_mem_HL (State *state)
{
  DReg HL = REG_HL(state);
  instr_SWAP_n(state->memory[HL], state);
  writeMem(HL, state->memory[HL], state); // this is done to use the special write function
  return 16;
}

// CPL: flip all bits in register A
inline int instr_CPL (State *state)
{
  SET_SUBTRACT_FLAG(state);
  SET_HALF_CARRY_FLAG(state);
  state->A = ~(state->A);
  return 4;
}

// complement = false: SCF: set carry flag
// complement = true:  CCF: flip carry flag
inline int instr_CCF_SCF (State *state, bool complement)
{
  RESET_SUBTRACT_FLAG(state);
  RESET_HALF_CARRY_FLAG(state);
  if (complement) {
    state->F ^= CARRY_FLAG;
  } else {
    SET_CARRY_FLAG(state);
  }
  return 4;
}

// RLC n: rotate n left, old bit 7 to carry flag
inline int instr_RLC_n (Reg &n, State *state)
{
  CLEAR_ALL_FLAGS(state);
  COND_SET_CARRY_FLAG(state, (n & 0x80) != 0);
  n = n << 1;
  COND_SET_ZERO_FLAG(state, n == 0);
  return 8;
}

// RLC (HL)
inline int instr_RLC_mem_HL (State *state)
{
  DReg HL = REG_HL(state);
  instr_RLC_n(state->memory[HL], state);
  writeMem(HL, state->memory[HL], state); // this is done to use the special write function
  return 16;
}

// RLC A (only opcode 0x07, not 0xCB 07 version)
inline int instr_RLC_A (State *state)
{
  instr_RLC_n(state->A, state);
  return 4;
}

// RL n: rotate left through carry flag
inline int instr_RL_n (Reg &n, State *state)
{
  Byte old_carry = GET_CARRY_FLAG(state);
  CLEAR_ALL_FLAGS(state);
  COND_SET_CARRY_FLAG(state, (n & 0x80) != 0);
  n = (n << 1) | old_carry;
  COND_SET_ZERO_FLAG(state, n == 0);
  return 8;
}

// RL (HL)
inline int instr_RL_mem_HL (State *state)
{
  DReg HL = REG_HL(state);
  instr_RL_n(state->memory[HL], state);
  writeMem(HL, state->memory[HL], state); // this is done to use the special write function
  return 16;
}

// RL A: rotate left through carry flag (only opcode 0x17, not 0xCB 17 version)
inline int instr_RL_A (State *state)
{
  instr_RL_n(state->A, state);
  return 4;
}

// RRC n: rotate n right, old bit 0 to carry flag
inline int instr_RRC_n (Reg &n, State *state)
{
  CLEAR_ALL_FLAGS(state);
  COND_SET_CARRY_FLAG(state, (n & 0x01) != 0);
  n = n >> 1;
  COND_SET_ZERO_FLAG(state, n == 0);
  return 8;
}

// RRC (HL)
inline int instr_RRC_mem_HL (State *state)
{
  DReg HL = REG_HL(state);
  instr_RRC_n(state->memory[HL], state);
  writeMem(HL, state->memory[HL], state); // this is done to use the special write function
  return 16;
}

// RRC A (only opcode 0x0F, not 0xCB 0F version)
inline int instr_RRC_A (State *state)
{
  instr_RRC_n(state->A, state);
  return 4;
}

// RR n: rotate right through carry flag
inline int instr_RR_n (Reg &n, State *state)
{
  Byte old_carry = GET_CARRY_FLAG(state);
  CLEAR_ALL_FLAGS(state);
  COND_SET_CARRY_FLAG(state, (n & 0x01) != 0);
  n = (n >> 1) | (old_carry*0x80);
  COND_SET_ZERO_FLAG(state, n == 0);
  return 8;
}

// RR (HL)
inline int instr_RR_mem_HL (State *state)
{
  DReg HL = REG_HL(state);
  instr_RR_n(state->memory[HL], state);
  writeMem(HL, state->memory[HL], state); // this is done to use the special write function
  return 16;
}

// RR A: rotate right through carry flag (only opcode 0x1F, not 0xCB 1F version)
inline int instr_RR_A (State *state)
{
  instr_RL_n(state->A, state);
  return 4;
}

// SLA n: shift left arithmetical into carry
inline int instr_SLA_n (Reg &reg, State *state)
{
  CLEAR_ALL_FLAGS(state);
  COND_SET_CARRY_FLAG(state, (reg & 0x80) != 0);
  reg = reg << 1;
  COND_SET_ZERO_FLAG(state, reg == 0);
  return 8;
}

// SLA (HL)
inline int instr_SLA_mem_HL (State *state)
{
  DReg HL = REG_HL(state);
  instr_SLA_n(state->memory[HL], state);
  writeMem(HL, state->memory[HL], state); // this is done to use the special write function
  return 16;
}

// SRA n: shift right into carry
inline int instr_SRA_n (Reg &reg, State *state)
{
  CLEAR_ALL_FLAGS(state);
  COND_SET_CARRY_FLAG(state, reg & 0x01);
  reg = Byte(SByte(reg) >> 1);
  COND_SET_ZERO_FLAG(state, reg == 0);
  return 8;
}

// SRA (HL)
inline int instr_SRA_mem_HL (State *state)
{
  DReg HL = REG_HL(state);
  instr_SRA_n(state->memory[HL], state);
  writeMem(HL, state->memory[HL], state); // this is done to use the special write function
  return 16;
}

// SRL n: logical shift n right into carry
inline int instr_SRL_n (Reg &reg, State *state)
{
  CLEAR_ALL_FLAGS(state);
  COND_SET_CARRY_FLAG(state, reg & 0x01);
  reg = reg >> 1;
  COND_SET_ZERO_FLAG(state, reg == 0);
  return 8;
}

// SRL (HL)
inline int instr_SRL_mem_HL (State *state)
{
  DReg HL = REG_HL(state);
  instr_SRL_n(state->memory[HL], state);
  writeMem(HL, state->memory[HL], state); // this is done to use the special write function
  return 16;
}

// BIT b, r: test bit b in register r
template <int b>
inline int instr_BIT_b_r (Reg &reg, State *state)
{
  constexpr Byte bit_mask = 1 << b;
  RESET_SUBTRACT_FLAG(state);
  SET_HALF_CARRY_FLAG(state);
  RESET_ZERO_FLAG(state);
  COND_SET_ZERO_FLAG(state, (reg & bit_mask) != 0);
  return 8;
}

// BIT b, (HL)
template <int b>
inline int instr_BIT_b_mem_HL (State *state)
{
  DReg HL = REG_HL(state);
  instr_BIT_b_r<b>(state->memory[HL], state);
  writeMem(HL, state->memory[HL], state); // this is done to use the special write function
  return 16;
}

// SET b, r: set bit b in register r
template<int b>
inline int instr_SET_b_r (Reg &reg, State *state)
{
  constexpr Byte bit_mask = 1 << b;
  reg |= bit_mask;
  return 8;
}

// SET b, (HL)
template<int b>
inline int instr_SET_b_mem_HL (State *state)
{
  DReg HL = REG_HL(state);
  instr_SET_b_r<b>(state->memory[HL], state);
  writeMem(HL, state->memory[HL], state); // this is done to use the special write function
  return 16;
}

// RES b, r: reset bit b in register r
template<int b>
inline int instr_RES_b_r (Reg &reg, State *state)
{
  constexpr Byte bit_mask = Byte(~(1 << b));
  reg &= bit_mask;
  return 8;
}

// RES b, (HL)
template<int b>
inline int instr_RES_b_mem_HL (State *state)
{
  DReg HL = REG_HL(state);
  instr_RES_b_r<b>(state->memory[HL], state);
  writeMem(HL, state->memory[HL], state); // this is done to use the special write function
  return 16;
}