/*
* This file handles the instruction execution
*/

#pragma once


#include <utility>
#include "emulator/state.h"
#include "emulator/interface.h"

#define CLOCK_FREQ 4194304
#define INSTR_BLOCK_N 10000 // Instructions are executed in groups of X
#define INSTRS_PER_BUTTON_UPDATE 64 // Each X instructions the new buttons pressed check is run


void initializeState (State *state);

// Execute instructions. If execution flow is interrupted earlier, function returns automatically.
void execute (State *state, Interface *interface);

inline void checkAndCallInterrupt (State *state)
{
  Byte if_reg = state->memory[IF_REGISTER];
  // Check first if there is any interrupt
  if (if_reg == 0) {
    return;
  }

  // Check interrupts one by one, if some interrupt is triggered return (don't check others)
  for (Byte i = 0; i < 5; i++) {
    Byte interrupt_flag = 1 << i;
    // If both IE and IF's corresponding bits are set to one
    if (if_reg & interrupt_flag) {
      state->ime = false;
      state->memory[IF_REGISTER] = state->memory[IF_REGISTER] & (~interrupt_flag); // clear interrupt bit
      // Push PC to stack
      state->memory[--state->SP] = state->PC >> 8;
      state->memory[--state->SP] = state->PC & 0xFF;
      // Set new value of PC
      switch (interrupt_flag)
      {
      case VBLANK_INTERRUPT:
        state->PC = 0x0040;
        break;
      case LCD_INTERRUPT:
        state->PC = 0x0048;
        break;
      case TIMER_INTERRUPT:
        state->PC = 0x0050;
        break;
      case SERIAL_INTERRUPT:
        state->PC = 0x0058;
        break;
      case JOYPAD_INTERRUPT:
        state->PC = 0x0060;
        break;
      default:
        break;
      }
      return;
    }
  }
}