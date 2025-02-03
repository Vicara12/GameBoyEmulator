/*
* Functions provided to emulator's Interface struct to couple it to the PC version.
*/

#pragma once

#include <array>
#include "emulator/multimedia/graphicstate.h"

using ScreenPixels = std::vector<std::vector<float>>;


inline void screenFrameToScreenPixels (ScreenFrame *frame, ScreenPixels &pixels)
{
  for (Byte row = 0; row < SCREEN_PX_H; row++) {
    for (Byte col = 0; col < SCREEN_PX_W; col++) {
      pixels[row][col] = frame->line[row].pixel[col];
    }
  }
}