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

// immediate = true  and carry = false: ADD A, n: add 8 bit immediate n to register A
// immediate = false and carry = false: ADD A, r: add register r to register A
// immediate = true  and carry = true: ADC A, n: add 8 bit immediate n + carry flag to register A
// immediate = false and carry = true: ADC A, r: add register r + carry flag to register A 
inline int instr_ADX_A_r (Reg r, State *state, bool immediate, bool carry)
{
  r += ((state->F & CARRY_FLAG) != 0)*carry;
  CLEAR_ALL_FLAGS(state);
  handleFlags(state->A, r, state, [](Short a, Short b)->Short {return a+b;});
  state->A += r;
  return 4 + 4*immediate;
}

// carry = false: ADD A, (HL): add content of memory at address HL into register A
// carry = true:  ADC A, (HL): add content of memory at address HL + carry into register A
inline int instr_ADX_A_mem_HL (State *state, bool carry)
{
  Byte r = state->memory[REG_HL(state)];
  r += ((state->F & CARRY_FLAG) != 0)*carry;
  CLEAR_ALL_FLAGS(state);
  handleFlags(state->A, r, state, [](Short a, Short b)->Short {return a+b;});
  state->A += r;
  return 8;
}

// immediate = true  and carry = false: SUB A, n: subtract 8 bit immediate n to register A
// immediate = false and carry = false: SUB A, r: subtract register r to register A
// immediate = true  and carry = true: SBC A, n: subtract 8 bit immediate n + carry flag to register A
// immediate = false and carry = true: SBC A, r: subtract register r + carry flag to register A 
inline int instr_SBX_A_r (Reg r, State *state, bool immediate, bool carry)
{
  r += ((state->F & CARRY_FLAG) != 0)*carry;
  CLEAR_ALL_FLAGS(state);
  SET_SUBTRACT_FLAG(state);
  handleFlags(state->A, r, state, [](Short a, Short b)->Short {return a-b;});
  state->A -= r;
  return 4 + 4*immediate;
}

// carry = false: SUB A, (HL): subtract content of memory at address HL into register A
// carry = true:  SBC A, (HL): subtract content of memory at address HL + carry into register A
inline int instr_SBX_A_mem_HL (State *state, bool carry)
{
  Byte r = state->memory[REG_HL(state)];
  r += ((state->F & CARRY_FLAG) != 0)*carry;
  CLEAR_ALL_FLAGS(state);
  SET_SUBTRACT_FLAG(state);
  handleFlags(state->A, r, state, [](Short a, Short b)->Short {return a-b;});
  state->A -= r;
  return 8;
}

// immediate = true  and type = 0: AND n: and 8 bit immediate n with register A
// immediate = false and type = 0: AND r: and register r or (HL) with register A
// immediate = true  and type = 1: OR n: or 8 bit immediate n with register A
// immediate = false and type = 1: OR r: or register r or (HL) with register A
// immediate = true  and type = 2: XOR n: xor 8 bit immediate n with register A
// immediate = false and type = 2: XOR r: xor register r or (HL) with register A
inline int instr_AND_OR_XOR_A_r (Reg r, State *state, bool immediate, bool use_hl, int type)
{
  if (use_hl) {
    r = state->memory[REG_HL(state)];
  }
  CLEAR_ALL_FLAGS(state);
  switch (type)
  {
    case 0:
      SET_HALF_CARRY_FLAG(state);
      state->A &= r;
      break;
    case 1:
      state->A |= r;
      break;
    case 2:
      state->A ^= r;
      break;
    default:
      return -1;
  }
  COND_SET_ZERO_FLAG(state, state->A == 0);
  return 4 + 4*(immediate or use_hl);
}