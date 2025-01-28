#pragma once

#include "emulator/types.h"
#include "emulator/generaldefines.h"
#include "emulator/state.h"


inline void writeMem (Short addr, Byte data, State* state)
{
  // If write in locked video memory region do not write
  if (LCDC_LCD_ENABLED(state) and (
      (not state->vram_write_enabled and addr >= 0x8000 and addr < 0xA000) or
      (not state->oam_write_enabled  and addr >= 0xFE00 and addr < 0xFEA0))) {
    return;
  }
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
  // Perform DMA data transfer if a value is written to DMA
  else if (addr == DMA_REGISTER) {
    Short base_addr = Short(data) << 8;
    for (Short i = 0; i < 0x00A0; i++) {
      state->memory[0xFE00 + i] = state->memory[base_addr];
    }
  }
}