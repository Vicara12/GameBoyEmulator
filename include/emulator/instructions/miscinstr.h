#pragma once

/*
* Code for several instructions that don't belong to a particular class.
*/

#include "emulator/state.h"

// DAA: decimal adjust register A
inline int instr_DAA (State *state)
{
  bool hc_flag = GET_HALF_CARRY_FLAG(state);
  bool c_flag = GET_CARRY_FLAG(state);
  bool sub_flag = GET_SUBTRACT_FLAG(state);
  Byte offset = 0x00;
  offset |= 0x06*(((not sub_flag) and ((state->A & 0x0F) > 0x09)) or hc_flag);
  offset |= 0x60*(((not sub_flag) and (state->A > 0x99)) or c_flag);
  RESET_ZERO_FLAG(state);
  RESET_HALF_CARRY_FLAG(state);
  if (sub_flag) {
    COND_SET_CARRY_FLAG(state, (Short(state->A) - offset) > 0xFF);
    state->A -= offset;
  } else {
    COND_SET_CARRY_FLAG(state, (Short(state->A) + offset) > 0xFF);
    state->A += offset;
  }
  COND_SET_CARRY_FLAG(state, state->A == 0);
  return 4;
}