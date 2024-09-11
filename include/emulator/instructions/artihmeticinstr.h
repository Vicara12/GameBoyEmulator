#pragma once

/*
* Code for all the arithmetic instructions (ADD)
*/

#include "emulator/state.h"


// When using Byte data type the function given to f should use data type Short (2 bytes), as the
// calculation for the carry requires more precision
template<typename FuncType>
inline void handleFlags (Byte a, Byte b, State *state, FuncType f)
{
  COND_SET_ZERO_FLAG(state, Byte(f(a, b)) == 0);
  COND_SET_HALF_CARRY_FLAG(state, (f((a & 0x0F), (b & 0x0F)) & 0xF0) != 0);
  COND_SET_CARRY_FLAG(state, (f(a, b) & 0xFF00) != 0);
}

// immediate = true: ADD A, n: add 8 bit immediate n to register A
// immediate = false: ADD A, r: add register r to register A 
inline int instr_ADD_A_r (Reg r, State *state, bool immediate)
{
  CLEAR_ALL_FLAGS(state);
  handleFlags(state->A, r, state, [](Short a, Short b)->Short {return a+b;});
  state->A += r;
  return 4 + 4*immediate;
}

// ADD A, (HL): add content of memory at address HL into register A
inline int instr_ADD_A_mem_HL (State *state)
{
  CLEAR_ALL_FLAGS(state);
  Byte r = state->memory[REG_HL(state)];
  handleFlags(state->A, r, state, [](Short a, Short b)->Short {return a+b;});
  state->A += r;
  return 8;
}