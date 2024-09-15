#pragma once

/*
* Code for all the bit arithmetic instructions
*/

#include "emulator/state.h"

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
  instr_SWAP_n(state->memory[REG_HL(state)], state);
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
  instr_RLC_n(state->memory[REG_HL(state)], state);
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
  instr_RL_n(state->memory[REG_HL(state)], state);
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
  instr_RRC_n(state->memory[REG_HL(state)], state);
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
  instr_RR_n(state->memory[REG_HL(state)], state);
  return 16;
}

// RR A: rotate right through carry flag (only opcode 0x1F, not 0xCB 1F version)
inline int instr_RR_A (State *state)
{
  instr_RL_n(state->A, state);
  return 4;
}