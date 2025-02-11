#include "emulator/emulator.h"
#include "emulator/types.h"
#include "emulator/state.h"
#include "emulator/interface.h"
#include "emulator/cpu/cpu.h"
#include "emulator/utils/initialization.h"
#include "emulator/utils/debug.h"



void emulator (Interface *interface, GameRom *game_rom, bool debug)
{
  State *state = new State;
  // Boot sequence
  loadBootRom(state);
  loadGame(state, game_rom);
  execute(state, interface, /* breakpoint = */ 0x0100);
  replaceBootRom(state, game_rom);
  // Run game
  if (debug) {
    runInDebug(state, interface);
  } else {
    execute(state, interface, /* no breakpoint */ 0xFFFF);
  }
}