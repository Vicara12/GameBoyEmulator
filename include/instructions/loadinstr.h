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

// LD r1, (r2): put value in memory location (r2) into register r1
inline int instr_LD_r1_mem_r2 (Reg &r1, DReg r2, State &state)
{
  r1 = state.memory[r2];
  return 8;
}

// LD (r1), r2: put value in nn (either register or 8 bit immediate) into memory location (r1)
inline int instr_LD_mem_r1_nn (DReg r1, Byte nn, State &state, bool immediate)
{
  state.memory[r1] = nn;
  return 8 +  4*immediate;
}