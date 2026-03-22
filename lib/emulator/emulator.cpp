#include "emulator.h"
#include "types.h"
#include "state.h"
#include "interface.h"
#include "cpu/cpu.h"
#include "utils/initialization.h"
#include "utils/debug.h"



void emulator (Interface *interface, GameRom *game_rom, bool debug)
{
  State *state = new State;
  ExecutionDebug edb;
  edb.breakpoint = 0x0100; // Set breakpoint at the end of boot rom
  // Boot sequence
  loadBootRom(state);
  loadGame(state, game_rom);
  execute(state, interface, edb);
  replaceBootRom(state, game_rom);
  // Run game
  if (debug) {
    runInDebug(state, interface);
  } else {
    execute(state, interface);
  }
}