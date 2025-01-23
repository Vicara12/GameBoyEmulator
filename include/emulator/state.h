
/*
* Code for everything related to memory and registers
*/

#pragma once

#include "emulator/types.h"
#include "emulator/interface.h"

#define MEM_SIZE 0x10000


// Button utils
#define BUTTON_RIGHT  0x01
#define BUTTON_LEFT   0x02
#define BUTTON_UP     0x04
#define BUTTON_DOWN   0x08
#define BUTTON_A      0x10
#define BUTTON_B      0x20
#define BUTTON_SEL    0x40
#define BUTTON_START  0x80


// Special registers
#define P1_REGISTER   0xFF00 // Controller input data
#define DIV_REGISTER  0xFF04 // Clock which is updated at 16384Hz (one overflow per second)
#define TIMA_REGISTER 0xFF05 // Clock incremented at freq specified by TAC, at of TMA is written
#define TMA_REGISTER  0xFF06 // Value written to TIMA at overflow
#define TAC_REGISTER  0xFF07 // Timer control

// Get TAC fields
#define GET_TAC_CLOCK_SEL(value_TAC) (value_TAC & 0x03)
#define GET_TAC_ENABLE(value_TAC) ((value_TAC & 0x4) != 0)


// Flag setting utils
#define ZERO_FLAG       0x80
#define SUBTRACT_FLAG   0x40
#define HALF_CARRY_FLAG 0x20
#define CARRY_FLAG      0x10

#define GET_ZERO_FLAG(state)       ((state->F & ZERO_FLAG) != 0)
#define GET_SUBTRACT_FLAG(state)   ((state->F & SUBTRACT_FLAG) != 0)
#define GET_HALF_CARRY_FLAG(state) ((state->F & HALF_CARRY_FLAG) != 0)
#define GET_CARRY_FLAG(state)      ((state->F & CARRY_FLAG) != 0)

#define CLEAR_ALL_FLAGS(state) state->F = 0x00

#define SET_ZERO_FLAG(state) state->F |= ZERO_FLAG
#define COND_SET_ZERO_FLAG(state, condition) state->F |= ZERO_FLAG*(condition)
#define RESET_ZERO_FLAG(state) state->F &= ~ZERO_FLAG

#define SET_SUBTRACT_FLAG(state) state->F |= SUBTRACT_FLAG
#define COND_SET_SUBTRACT_FLAG(state, condition) state->F |= SUBTRACT_FLAG*(condition)
#define RESET_SUBTRACT_FLAG(state) state->F &= ~SUBTRACT_FLAG

#define SET_HALF_CARRY_FLAG(state) state->F |= HALF_CARRY_FLAG
#define COND_SET_HALF_CARRY_FLAG(state, condition) state->F |= HALF_CARRY_FLAG*(condition)
#define RESET_HALF_CARRY_FLAG(state) state->F &= ~HALF_CARRY_FLAG

#define SET_CARRY_FLAG(state) state->F |= CARRY_FLAG
#define COND_SET_CARRY_FLAG(state, condition) state->F |= CARRY_FLAG*(condition)
#define RESET_CARRY_FLAG(state) state->F &= ~CARRY_FLAG

// Double register utils
#define JOIN_REGS(r1, r2) ((DReg(r1) << 8) | DReg(r2))
#define REG_AF(state) JOIN_REGS(state->A, state->F)
#define REG_BC(state) JOIN_REGS(state->B, state->C)
#define REG_DE(state) JOIN_REGS(state->D, state->E)
#define REG_HL(state) JOIN_REGS(state->H, state->L)

#define STORE_SHORT(value, dst_high, dst_low) dst_high = Byte(value >> 8); dst_low = Byte(value & 0xFF)
#define SET_REG_AF(value, state) STORE_SHORT(value, state->A, state->F)
#define SET_REG_BC(value, state) STORE_SHORT(value, state->B, state->C)
#define SET_REG_DE(value, state) STORE_SHORT(value, state->D, state->E)
#define SET_REG_HL(value, state) STORE_SHORT(value, state->H, state->L)

// Memory access utils
#define SET_INTERRUPT_STATUS(value, state) state->memory[0xFFFF] = value;


ulong getDivFromTAC (Byte value_TAC);


typedef struct {
  Reg A = 0;
  Reg B = 0;
  Reg C = 0;
  Reg D = 0;
  Reg E = 0;
  Reg F = 0;
  Reg H = 0;
  Reg L = 0;
  DReg SP = 0xFFFE;
  DReg PC = 0x0000;
  Byte memory[MEM_SIZE] = {0};
  bool halted = false;
  bool stopped = false;
  Byte buttons_pressed = 0; // The emulator should call the function that checks this frequently
  ulong cycles = 0; // Total execution cycles (execution clock)
  ulong cycles_last_DIV = 0;  // Counts the execution cycles of the last write to DIV
  ulong cycles_last_TIMA = 0; // Cycles of last TIMA overflow
  ulong cycles_div_TIMA = getDivFromTAC(0);
  bool enable_TIMA = false;
} State;


inline void writeMem (Short addr, Byte data, State* state)
{
  state->memory[addr] = data;
  // Check write on RAM, if so write both on orig and mirror
  if (addr >= 0xC000 and addr < 0xDE00) {
    state->memory[addr-0xC000+0xE000] = data;
  }
  else if (addr >= 0xE000 and addr < 0xFE00) {
    state->memory[addr-0xE000+0xC000] = data;
  }
  // Check write to numpad register 0xFF00 and some combination is to be read (bits 4 or 5 to LOW),
  // if so update contents with numpad input
  else if (addr == P1_REGISTER and (data & 0x30) != 0x30) {
    bool read_letters = (data & 0x20 == 0); // low at bit 5 indicates A/B/Sel/Start read
    // If we want to read arrows then we keep 4 lower bits, otherwise we want the 4 upper bits of
    // buttons_pressed as the 4 lower bits of input
    Byte input = (state->buttons_pressed >> (4*read_letters)) & 0x0F;
    // Update input bits with numpad input. Recall that button pressed means LOW (0) and vice versa
    state->memory[0xFF00] = 0x30 | (0x0F & (~input));
  }
  // If write to DIV (0xFF04) register, set its value to zero
  else if (addr == DIV_REGISTER) {
    state->cycles_last_DIV = state->cycles; // it will be set to zero next time its updated
  }
  // If write to TAC, change state with TIMA enabled/disabled and clock div
  else if (addr == TAC_REGISTER) {
    state->enable_TIMA = GET_TAC_ENABLE(data);
    state->cycles_div_TIMA = getDivFromTAC(data);
  }
}