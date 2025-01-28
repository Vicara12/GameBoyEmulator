/*
* Several defines and structures regarding graphics
*/

#pragma once

#include <array>
#include "emulator/types.h"


// Timing constants
#define DOTS_PER_FRAME  70224
#define DOTS_PER_LINE   456
#define DOTS_OAM_SCAN   80
#define DOTS_DRAW_PX    200

// Screen constants
#define SCREEN_PX_W     160
#define SCREEN_PX_H     144
#define LINES_IN_SCREEN 154
#define BG_PX_SIZE      256

// Registers
#define LCDC_REGISTER 0xFF40 // LCD Control
#define STAT_REGISTER 0xFF41 // LCD Status
#define LY_REGISTER   0xFF44 // LCD Y coordinate
#define LYC_REGISTER  0xFF45 // LY compare
#define DMA_REGISTER  0xFF46 // Transfer from ROM/RAM to OAM

#define LCDC_LCD_ENABLED(state)         (state->memory[LCDC_REGISTER] & 0x80 != 0)
#define LCDC_WIN_TILE_MAP(state)        (state->memory[LCDC_REGISTER] & 0x40 != 0)
#define LCDC_WIN_ENABLE(state)          (state->memory[LCDC_REGISTER] & 0x20 != 0)
#define LCDC_BG_W_TILE_DATA_AREA(state) (state->memory[LCDC_REGISTER] & 0x10 != 0)
#define LCDC_BG_TILE_MAP(state)         (state->memory[LCDC_REGISTER] & 0x08 != 0)
#define LCDC_OBJ_SIZE(state)            (state->memory[LCDC_REGISTER] & 0x04 != 0)
#define LCDC_OBJ_ENABLE(state)          (state->memory[LCDC_REGISTER] & 0x02 != 0)
#define LCDC_BG_WIN_ENABLE(state)       (state->memory[LCDC_REGISTER] & 0x01 != 0)

using ScreenLineData = std::array<Byte, SCREEN_PX_W>;


typedef struct {
  ulong frame_last_updated = 0;
  ScreenLineData pixel;
} ScreenLine;

typedef struct {
  ulong frame_last_updated = 0;
  std::array<ScreenLine, SCREEN_PX_H> line;
} ScreenFrame;