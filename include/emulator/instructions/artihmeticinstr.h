#pragma once

/*
* Code for all the arithmetic instructions
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

// CP r: compare A with register, 8 bit value or (HL)
inline int instr_CP_r (Reg r, State *state, bool immediate, bool use_hl)
{
  if (use_hl) {
    r = state->memory[REG_HL(state)];
  }
  CLEAR_ALL_FLAGS(state);
  SET_SUBTRACT_FLAG(state);
  handleFlags(state->A, r, state, [](Short a, Short b)->Short {return a-b;});
  return 4 + 4*(immediate or use_hl);
}

// dec = false: INC r: increment register
// dec = true:  DEC r: decrement register
inline int instr_INC_DEC_r (Reg &r, State *state, bool dec)
{
  // can't do clear all as caryy flag can't be affected
  RESET_HALF_CARRY_FLAG(state);
  if (dec) {
    r--;
    SET_SUBTRACT_FLAG(state);
    COND_SET_HALF_CARRY_FLAG(state, (r & 0x0F) == 0xF);
  } else {
    r++;
    RESET_SUBTRACT_FLAG(state);
    COND_SET_HALF_CARRY_FLAG(state, (r & 0x0F) == 0x0);
  }
  RESET_ZERO_FLAG(state);
  COND_SET_ZERO_FLAG(state, r == 0);
  return 4;
}

// dec = false: INC (HL): increment (HL)
// dec = true:  DEC (HL): decrement (HL)
inline int instr_INC_DEC_mem_HL (State *state, bool dec)
{
  instr_INC_DEC_r(state->memory[REG_HL(state)], state, dec);
  return 12;
}

// ADD HL, n: add two byte register n to HL
inline int instr_ADD_HL_n (DReg reg, State *state)
{
  RESET_SUBTRACT_FLAG(state);
  RESET_HALF_CARRY_FLAG(state);
  RESET_CARRY_FLAG(state);
  DReg HL = REG_HL(state);
  COND_SET_HALF_CARRY_FLAG(state, ((HL & 0x0FFF) + (reg & 0x0FFF)) > 0x0FFF);
  COND_SET_CARRY_FLAG(state, (uint32_t(reg) + uint32_t(HL)) > 0xFFFF);
  SET_REG_HL(HL + reg, state);
  return 8;
}

// ADD SP, n: add one byte signed immediate to SP
inline int instr_ADD_SP_n (SByte n, State *state)
{
  CLEAR_ALL_FLAGS(state);
  COND_SET_HALF_CARRY_FLAG(state, (((state->SP & 0x0FFF) + n) & 0xF000) != 0);
  COND_SET_CARRY_FLAG(state, ((uint32_t(state->SP) + n) & 0xFFFF0000) != 0);
  state->SP += n;
  return 16;
}

// dec = false: INC nn: increment two byte register
// dec = true:  DEC nn: decrement two byte register
inline int instr_INC_DEC_nn_16bits (Reg &reg_high, Reg &reg_low, bool dec)
{
  DReg nn = JOIN_REGS(reg_high, reg_low);
  if (dec) {
    nn--;
  } else {
    nn++;
  }
  STORE_SHORT(nn, reg_high, reg_low);
  return 8;
}

// dec = false: INC SP: increment stack pointer
// dec = true:  DEC SP: decrement stack pointer
inline int instr_INC_DEC_SP (State *state, bool dec)
{
  if (dec) {
    state->SP--;
  } else {
    state->SP++;
  }
  return 8;
}