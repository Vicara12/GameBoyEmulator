/*
* Functions provided to emulator's Interface struct to couple it to the PC version.
*/

#pragma once

#include <array>
#include <iostream>
#include <mutex>
#include <vector>
#include "emulator/interface.h"
#include "emulator/multimedia/graphicstate.h"


using ScreenPixels = std::vector<std::vector<float>>;


// DO NOT set or read values from this struct, use the provided functions instead
typedef struct {
  std::mutex screen_mutex;
  ScreenPixels screen = ScreenPixels(SCREEN_PX_H, std::vector<float>(SCREEN_PX_W));
  Byte buttons = 0x00;
  std::chrono::steady_clock::time_point ini_t;
  bool ini_t_initialized = false;
} InterfaceData;


// Functions used by main program

// Returns the initialized interface and interface data structs
std::pair<Interface*,InterfaceData*> getInterface ();

void updateButtons (Byte buttons_pressed);

void getNewScreen (ScreenPixels &sp);


// Functions used by the emulator's interface

inline std::chrono::steady_clock::time_point timeNow ()
{
  return std::chrono::steady_clock::now();
}


inline ulong timeMicros (InterfaceData *if_data)
{
  if (not if_data->ini_t_initialized) {
    if_data->ini_t = timeNow();
    if_data->ini_t_initialized = true;
  }
  return std::chrono::duration_cast<std::chrono::microseconds>(timeNow() - if_data->ini_t).count();
}


inline void updateScreen (InterfaceData *if_data, ScreenFrame *sf)
{
  if_data->screen_mutex.lock();
  for (Byte row = 0; row < SCREEN_PX_H; row++) {
    for (Byte col = 0; col < SCREEN_PX_W; col++) {
      if_data->screen[row][col] = sf->line[row].pixel[col];
    }
  }
  if_data->screen_mutex.unlock();
}