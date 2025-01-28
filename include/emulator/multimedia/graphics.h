/*
* Functions for obtaining the color of each pixel in the screen (used to render graphics)
*/

#pragma once

#include "emulator/types.h"
#include "emulator/state.h"
#include "emulator/interface.h"
#include "emulator/multimedia/graphicstate.h"


// Mode 0 is HBlank, mode 1 is VBlank, mode 2 OAM scan (pre drawing) and mode 3 is drawing pixels
inline Byte getMode (State *state, Byte &current_line)
{
  ulong current_dot = state->cycles%DOTS_PER_FRAME;
  current_line = current_dot/DOTS_PER_LINE;
  if (current_line >= SCREEN_PX_H) {
    return 1; // VBlank
  }
  ulong dot_in_line = current_dot%DOTS_PER_LINE;
  if (dot_in_line < DOTS_OAM_SCAN) {
    return 2; // OAM scan
  }
  else if (dot_in_line < DOTS_OAM_SCAN + DOTS_DRAW_PX) {
    return 3; // drawing pixels
  }
  return 0; // HBlank
}

inline ScreenLineData renderLineOBJ (Byte line_n, State *state) {
  // TODO
}

inline ScreenLineData renderLineBGW (Byte line_n, State *state) {
  // TODO
}

inline void mergeRenderedLines (ScreenLineData &obj_line, ScreenLineData &bgw_line, State *state) {
  // TODO
}

inline void updateGraphics (State *state, Interface *interface)
{
  if (not LCDC_LCD_ENABLED(state)) {
    return;
  }

  Byte line_n;
  Byte mode = getMode(state, line_n);
  Byte stat_value = state->memory[STAT_REGISTER];
  // Check if LYC == LY interrupt needs to be triggered
  if ((stat_value & 0x40) != 0 and // LYC int select to 1
      state->memory[LYC_REGISTER] == line_n and // LYC == LY
      (stat_value & 0x04) == 0 // LYC == LY flag was not previously set
      ) {
    // TODO: trigger interrupt
  }
  // Update LCD status registers
  state->memory[LY_REGISTER] = line_n;
  stat_value &= 0xF8; // clear three lsb of stat
  stat_value |= mode; // update PPU mode to two lsb of stat
  stat_value |= Byte(state->memory[LYC_REGISTER] == line_n) << 2; // update LYC == LY flag
  state->memory[STAT_REGISTER] = stat_value;

  // Check if a new line needs to be rendered
  if (mode == 0 and // at HBlank
      state->screen.frame_last_updated != state->screen.line[line_n].frame_last_updated) {
    ScreenLineData obj_line = renderLineOBJ(line_n, state);
    ScreenLineData bgw_line = renderLineBGW(line_n, state);
    mergeRenderedLines(obj_line, bgw_line, state);

    // If rendered last line of frame, call screen update function
    if (line_n == SCREEN_PX_H-1) {
      interface->printToScreen(state->screen);
    }
  }
}