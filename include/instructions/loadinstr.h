#pragma once

#include "state.h"

// LD nn,n: put 8 bit immediate value n into register nn
inline int instr_LD_nn_n (Reg &nn, Byte n)
{
  nn = n;
  return 8;
}

// LD r1,r2: put value in register r2 (src) into register r1 (dst)
inline int instr_LD_r1_r2 (Reg &r1, Reg r2)
{
  r1 = r2;
  return 4;
}

// LD r1, (r2): put value in memory location (nn) (either register or 8 bit immediate) into register r1
inline int instr_LD_r1_mem_nn (Reg &r1, DReg nn, State &state, bool immediate)
{
  r1 = state.memory[nn];
  return 8 + 8*immediate;
}

// LD (r1), r2: put value in nn (either register or 8 bit immediate) into memory location (r1)
inline int instr_LD_mem_r1_nn (DReg r1, Byte nn, State &state, bool immediate)
{
  state.memory[r1] = nn;
  return 8 +  4*immediate;
}

// immediate = true:  LD A, (n): put value at address 0xFF00 + immediate n into register A
// immediate = false: LD A, (C): put value at address 0xFF00 + C into register A
inline int instr_LD_A_FF00_n (State &state, Byte n, bool immediate)
{
  state.A = state.memory[0xFF00 + n];
  return 8 + 4*immediate;
}

// immediate = true:  LD (n), A: put value of register A into address 0xFF00 + immediate n
// immediate = false: LD (C), A: put value of register A into address 0xFF00 + C
inline int instr_LD_FF00_n_A (State &state, Byte n, bool immediate)
{
  state.memory[0xFF00 + n] = state.A;
  return 8 + 4*immediate;
}

// A_src = true  and inc = false: LDD A, (HL): put value at memory address HL into A and decrement HL
// A_src = false and inc = false: LDD (HL), A: put value in A into memory address HL and decrement HL
// A_src = true  and inc = true:  LDD A, (HL): put value at memory address HL into A and increment HL
// A_src = false and inc = true:  LDD (HL), A: put value in A into memory address HL and increment HL
inline int instr_LDX_A_mem_HL (State &state, bool A_src, bool inc)
{
  DReg HL = REG_HL(state);
  if (A_src) {
    state.memory[HL] = state.A;
  } else {
    state.A = state.memory[HL];
  }
  if (inc) {
    HL++;
  } else {
    HL--;
  }
  SET_REG_HL(HL, state);
  return 8;
}

// LD n, nn: put 16 bit literal nn into 16 bit register n (except for SP)
inline int instr_LD_dReg_nn (Reg &upper_reg, Reg &lower_reg, Byte nn_upper, Byte nn_lower)
{
  lower_reg = nn_lower;
  upper_reg = nn_upper;
  return 12;
}

// LD SP, nn and LD SP, HL
inline int instr_LD_SP_nn (State &state, Short nn)
{
  state.SP = nn;
  return 12;
}

// LDHL SP, n: put SP + n into register HL
inline int instr_LDHL_SP_n (State &state, SByte n)
{
  Short HL = state.SP + SByte(n);
  SET_REG_HL(HL, state);
  RESET_ZERO_FLAG(state);
  RESET_SUBTRACT_FLAG(state);
  COND_SET_HALF_CARRY_FLAG(state, (((state.SP & 0x00FF) + n) & 0xFF00) != 0);
  COND_SET_CARRY_FLAG(state, ((uint32_t(state.SP) + n) & 0xFFFF) != 0);
  return 12;
}

// LD (nn), SP: store stack pointer at memory address specified by 16 bit immediate nn (lower byte first)
inline int instr_LD_mem_nn_SP (Short nn, State &state)
{
  STORE_SHORT(state.SP, state.memory[nn+1], state.memory[nn]);
  return 20;
}

// PUSH nn: push register pair nn onto stack and decrement SP twice
inline int instr_PUSH_nn (Reg upper_reg, Reg lower_reg, State &state)
{
  state.memory[state.SP--] = lower_reg;
  state.memory[state.SP--] = upper_reg;
  return 16;
}

// POP nn: pop two bytes off stack into register pair nn and increment SP twice
inline int instr_POP_nn (Reg &upper_reg, Reg &lower_reg, State &state)
{
  upper_reg = state.memory[state.SP--];
  lower_reg = state.memory[state.SP--];
  return 12;
}