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

// NOP: no operation
inline int instr_NOP ()
{
  return 4;
}

// HALT: power down CPU until an interrupt occurs
inline int instr_HALT (State *state)
{
  state->halted = true;
  return 4;
}

// STOP: halt CPU and LCD display until button pressed
inline int instr_STOP (State *state)
{
  state->halted = true;
  return 4;
}

// enable = false: DI: disable interrupts after instruction execution
// enable = true:  EI: enable interrupts after instruction execution
inline int instr_DI_EI (State *state, bool enable)
{
  state->interrupt_enabled = enable;
  return 4;
}