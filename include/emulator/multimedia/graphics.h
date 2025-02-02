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


inline PaletteColors colorsFromPalette(Byte palette, bool object)
{
  PaletteColors colors = {
    (object ? COLOR_TRANS : (palette & 0x03)),
    (palette & 0x0C) >> 2,
    (palette & 0x30) >> 4,
    (palette & 0xC0) >> 6
  };
  return colors;
}


// Get the colors of a tile's line.
// - addr: memory address of the tile in the tile map (0x9800 - 0x9BFF and 0x9C00 - 0x9FFF)
// - tile_line: number of the tile's line (0 to 7, both inclusive)
// - is_object: wether the tile corresponds to an object or BG/Window
// - use_obp0: if is_object wether to use palette OBJ0 or OBJ1, ignored otherwise
// - dst: memory address where the colors will be written, must be at least 8 bytes wide
// - state: emulator's state struct
inline void getTileLine (Short addr, Byte tile_line, bool is_object, bool use_obp0, Byte *dst, State *state)
{
  Short tile_data_address;
  // Compute tile data address from Block 0 (0x8000 - 0x87FF) and Block 1 (0x8800 - 0x8FFF)
  if (is_object or LCDC_BG_W_TILE_DATA_AREA_LOW(state)) {
    uint16_t tile_data_index = state->memory[addr];
    tile_data_address = 0x8000 + tile_data_index*16; // Each tile is 16 bytes
    // Modify addr if object tile in 8x16 mode
    if (is_object and LCDC_OBJ_SIZE_BIG(state)) {
      if (tile_line < 8) {
        tile_data_address &= 0xFE; // Set lsb to 0
      } else {
        tile_data_address |= 0x01; // Set lsb to 1
      }
    }
  }
  // Compute tile data address from Block 1 (0x8800 - 0x8FFF) and Block 2 (0x9000 - 0x97FF)
  else {
    int16_t tile_data_index = int8_t(state->memory[addr]);
    tile_data_address = 0x9000 + tile_data_index*16;
  }
  // Retrieve tile data and get palette indices from it
  std::array<Byte,2> tile_data;
  tile_data[0] = tile_data_address + tile_line*2;
  tile_data[1] = tile_data_address + tile_line*2 + 1;
  std::array<Byte,8> palette_ids;
  for (Byte i = 0; i < 8; i++) {
    Byte msb = ((tile_data[1] & (1 << (7-i))) != 0); // 7-i because lsb is tile's rightmost pixel
    Byte lsb = ((tile_data[0] & (1 << (7-i))) != 0);
    palette_ids[i] = msb*2 + lsb;
  }
  // Transform palette indices to actual colors
  Byte palette;
  if (is_object) {
    palette = state->memory[use_obp0 ? OBP0_REGISTER : OBP1_REGISTER];
  } else {
    palette = state->memory[BGP_REGISTER];
  }
  PaletteColors colors = colorsFromPalette(palette, is_object);
  for (Byte i = 0; i < 8; i++) {
    dst[i] = colors[palette_ids[i]];
  }
}


inline ScreenLineData renderLineOBJ (Byte line_n, State *state)
{
  // TODO
}


inline ScreenLineData renderLineBGW (Byte line_n, State *state)
{
  // If master BG and Window enable is off return white line
  if (not LCDC_BG_WIN_ENABLED(state)) {
    ScreenLineData line;
    line.fill(COLOR_WHITE);
    return line;
  }

  // Get the entire BG screen line from the 256x256 map
  std::array<Byte,32> bg_line;
  Byte scy = state->memory[SCY_REGISTER]; // Viewport Y position
  Byte scx = state->memory[SCX_REGISTER]; // Viewport X position
  Short base_addr_bg = (LCDC_BG_TILE_MAP_HIGH(state) ? 0x9C00 : 0x9800); // Select bg tile map area
  Byte bg_line_n = (scy + line_n)%256; // Y coord of the line relative to BG
  Byte tile_y = bg_line_n/8; // Y pos in the 32x32 bg's tile map
  Byte tile_line = bg_line_n%8; // Number of the line in the tile (0 to 7, both inclusive)
  Short addr_left_tile = base_addr_bg + tile_y*32; // Addr of the tile on the left
  for (Short i = 0; i < 32; i++) {
    getTileLine(addr_left_tile + i, tile_line, false, false, &bg_line + i*8, state);
  }

  ScreenLineData bg_w_viewport_line;
  // If enabled, get the window pixels and merge them with BG. Otherwise just copy BG pixels.
  if (LCDC_WIN_ENABLE(state)) {
    // TODO draw window
  } else {
    for (int i = 0; i < SCREEN_PX_W; i++) {
      bg_w_viewport_line[i] = bg_line[(i+scx)%256];
    }  
  }

  return bg_w_viewport_line;
}


inline void renderLine (Byte line_n, State *state)
{
  ScreenLineData obj_line = renderLineOBJ(line_n, state);
  ScreenLineData bgw_line = renderLineBGW(line_n, state);
  // Merge BG, Window and Object pixels and write to screen as float intensity
  for (Byte i = 0; i < SCREEN_PX_W; i++) {
    Byte pixel_color = (obj_line[i] == COLOR_TRANS ? bgw_line[i] : obj_line[i]);
    switch (pixel_color) {
      case COLOR_WHITE:
        state->screen.line[line_n].pixel[i] = 1.00;
      case COLOR_LGRAY:
        state->screen.line[line_n].pixel[i] = 0.66;
      case COLOR_DGRAY:
        state->screen.line[line_n].pixel[i] = 0.33;
      case COLOR_BLACK:
        state->screen.line[line_n].pixel[i] = 0.00;
    }
  }
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