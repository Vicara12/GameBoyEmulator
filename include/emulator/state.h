
/*
* Code for everything related to memory and registers
*/

#pragma once

#include "emulator/types.h"
#include "emulator/interface.h"
#include "emulator/generaldefines.h"
#include "emulator/multimedia/graphicstate.h"


ulong getDivFromTAC (Byte value_TAC);


typedef struct {
  float target_speed = 1.0;
  bool debug = false;
  bool end_emulation = false;
} EmulatorConfig;


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
  bool ime = false; // Interrupt Master Enable cpu flag
  ulong cycles = 0; // Total execution cycles (execution clock)
  ulong cycles_last_DIV = 0;  // Counts the execution cycles of the last write to DIV
  ulong cycles_last_TIMA = 0; // Cycles of last TIMA overflow
  ulong cycles_div_TIMA = getDivFromTAC(0);
  bool enable_TIMA = false;
  bool vram_write_enabled = true;
  bool oam_write_enabled = true;
  ulong last_rate_call = 0; // Last time interface->informEmuRate was called
  ScreenFrame screen;
  EmulatorConfig config;
} State;