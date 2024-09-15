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