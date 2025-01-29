/*
* Functions for obtaining the color of each pixel in the screen (used to render graphics)
*/

#pragma once

#include "emulator/types.h"
#include "emulator/state.h"
#include "emulator/interface.h"
#include "emulator/multimedia/graphicstate.h"


inline Byte getMode (State *state, Byte &current_line)
{
  ulong current_dot = state->cycles%DOTS_PER_FRAME;
  current_line = current_dot/DOTS_PER_LINE;
  if (current_line >= SCREEN_PX_H) {
    return MODE1_VBLANK;
  }
  ulong dot_in_line = current_dot%DOTS_PER_LINE;
  if (dot_in_line < DOTS_OAM_SCAN) {
    return MODE2_OAMSC;
  }
  else if (dot_in_line < DOTS_OAM_SCAN + DOTS_DRAW_PX) {
    return MODE3_DRAW;
  }
  return MODE0_HBLANK;
}

inline void setInterrupts (Byte line_n, Byte mode, State *state)
{
  Byte stat_value = state->memory[STAT_REGISTER];

  // Handle STAT (LCD) interrupts
  if (IS_INTERRUPT_ENABLED(state, LCD_INTERRUPT)) {
    // Check if LYC == LY interrupt needs to be triggered
    if (state->memory[LYC_REGISTER] == line_n) {
      if ((stat_value & 0x40) != 0 and // LYC int select to 1
          not state->screen.ly_lyc_flag_already_set
          ) {
        state->screen.ly_lyc_flag_already_set = true;
        state->memory[IF_REGISTER] &= LCD_INTERRUPT;
      }
    } else {
      state->screen.ly_lyc_flag_already_set = false;
    }
    // Check if mode interrupt needs to be triggered
    if ((mode != state->screen.last_mode) and 
        ((mode == MODE0_HBLANK and (stat_value & 0x08) != 0) or
         (mode == MODE1_VBLANK and (stat_value & 0x10) != 0) or
         (mode == MODE2_OAMSC  and (stat_value & 0x20) != 0))) {
      state->memory[IF_REGISTER] &= LCD_INTERRUPT;
    }
  }

  // Handle VBLank interrupts
  if (IS_INTERRUPT_ENABLED(state, VBLANK_INTERRUPT) and mode != state->screen.last_mode and
      mode == MODE1_VBLANK) {
    state->memory[IF_REGISTER] &= VBLANK_INTERRUPT;
  }

  state->screen.last_mode = mode;
}

inline void updateLCDStatusRegs (Byte line_n, Byte mode, State *state)
{
  Byte stat_value = state->memory[STAT_REGISTER];
  state->memory[LY_REGISTER] = line_n;
  stat_value &= 0xF8; // clear three lsb of stat
  stat_value |= mode; // update PPU mode to two lsb of stat
  stat_value |= Byte(state->memory[LYC_REGISTER] == line_n) << 2; // update LYC == LY flag
  state->memory[STAT_REGISTER] = stat_value;
}

inline ScreenLineData renderLineOBJ (Byte line_n, State *state)
{
  // TODO
}

inline ScreenLineData renderLineBGW (Byte line_n, State *state)
{
  // TODO
}

inline void mergeRenderedLines (ScreenLineData &obj_line, ScreenLineData &bgw_line, State *state)
{
  // TODO
}

inline void renderLine (Byte line_n, State *state)
{
  ScreenLineData obj_line = renderLineOBJ(line_n, state);
  ScreenLineData bgw_line = renderLineBGW(line_n, state);
  mergeRenderedLines(obj_line, bgw_line, state);
  // TODO
}

inline void updateGraphics (State *state, Interface *interface)
{
  if (not LCDC_LCD_ENABLED(state)) {
    return;
  }

  Byte line_n;
  Byte mode = getMode(state, line_n);
  setInterrupts(line_n, mode, state);
  updateLCDStatusRegs(line_n, mode, state);

  // Check if a new line needs to be rendered
  ulong current_frame = state->cycles/DOTS_PER_FRAME;
  if (mode == MODE0_HBLANK and current_frame != state->screen.line[line_n].frame_last_updated) {
    renderLine(line_n, state);
    // If rendered last line of frame and frame has been rendered from line 0, call screen update
    if (line_n == SCREEN_PX_H-1 and current_frame != state->screen.line[0].frame_last_updated) {
      interface->updateScreen(state->screen);
    }
    state->screen.line[line_n].frame_last_updated = current_frame;
  }
}