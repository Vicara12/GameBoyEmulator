/*
* Instruction execution handling
*/

#pragma once


#include <utility>
#include "emulator/state.h"
#include "emulator/interface.h"

#define INSTRS_PER_BUTTON_UPDATE 64 // Each X instructions the new buttons pressed check is run



// Execute instructions. If execution flow is interrupted earlier, function returns automatically.
void execute (State *state, Interface *interface, Short breakpoint = 0xFFFF);


inline void updateTimeRegisters (State *state)
{
  state->memory[DIV_REGISTER] = (state->cycles - state->cycles_last_DIV)/256;
  // TODO: reset this register when the STOP mode ends
  if (state->enable_TIMA) {
    Byte old_TIMA = state->memory[TIMA_REGISTER];
    state->memory[TIMA_REGISTER] = (state->cycles - state->cycles_last_TIMA)/state->cycles_div_TIMA;
    // Detect TIMA overflow
    if (old_TIMA > state->memory[TIMA_REGISTER]) {
      state->memory[TIMA_REGISTER] = state->memory[TMA_REGISTER];
      if (IS_INTERRUPT_ENABLED(state, TIMER_INTERRUPT)) {
        state->memory[IF_REGISTER] &= TIMER_INTERRUPT; // Set timer interrupt if enabled
      }
    }
  }
}


inline void updateButtons (ulong n_instrs, State *state, Interface *interface)
{
  if (n_instrs%INSTRS_PER_BUTTON_UPDATE == 0) {
    Byte old_buttons_pressed = state->buttons_pressed;
    state->buttons_pressed = interface->readButtons();
    // If any change in button pressed activate interrupt
    if (IS_INTERRUPT_ENABLED(state, JOYPAD_INTERRUPT) and
        (old_buttons_pressed ^ state->buttons_pressed) != 0) {
      state->memory[IF_REGISTER] &= JOYPAD_INTERRUPT;
    }
  }
}


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


inline void synchExecution (State *state, Interface *interface)
{
  float emulator_time = state->config.target_speed*(state->cycles)/CLOCK_FREQ;
  float diff_ms = std::abs(emulator_time - interface->realTimeMicros())/1000;
  if (diff_ms > 1) {
    interface->sleepMillis(diff_ms);
  }
}