#include "emulator/emulator.h"
#include "emulator/types.h"
#include "emulator/state.h"
#include "emulator/interface.h"
#include "emulator/cpu/cpu.h"
#include "emulator/utils/initialization.h"



void emulator (Interface *interface, GameRom *game_rom)
{
  State *state = new State;
  // Boot sequence
  loadBootRom(state);
  loadGame(state, game_rom);
  execute(state, interface, /* breakpoint = */ 0x0100);
  replaceBootRom(state, game_rom);
  // Run game
  execute(state, interface, /* no breakpoint */ 0xFFFF);
}