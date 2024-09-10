#pragma once

#include "state.h"

// LD nn,n: put 8 bit immediate value n into register nn
inline int instr_LD_nn_n (Reg &reg_dst, Byte n)
{
  reg_dst = n;
  return 2;
}