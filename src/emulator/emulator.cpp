#include "emulator/emulator.h"
#include "emulator/state.h"
#include "emulator/interface.h"
#include "emulator/cpu/cpu.h"
#include "emulator/utils/initialization.h"



void boot (State *state, Interface *interface, GameRom *game_rom)
{
  loadBootRom(state);
  loadGame(state, game_rom);
  execute(state, interface, /* breakpoint = */ 0x0100);
}


void run (State *state, Interface *interface)
{
  // TODO
}

void emulator (Interface *interface, GameRom *game_rom = nullptr)
{
  State *state = new State;
  boot(state, interface, game_rom);
  run(state, interface);
}