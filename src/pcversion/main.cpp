#include <SFML/Graphics.hpp>
#include <thread>
#include "pcversion/interfaceadapter.h"
#include "pcversion/program.h"
#include "emulator/emulator.h"
#include "tetris.h"



int main (int argc, char **argv)
{
  GameRom game_rom;
  if (argc > 1) {
    // Read game rom, if there was an error quit emulator
    if (not readRom(std::string(argv[1]), game_rom)) {
      return 1;
    }
  } else {
    std::copy(tetris_rom.begin(), tetris_rom.end(), game_rom.begin());
  }
  std::pair<Interface*,InterfaceData*> if_pair = getInterface();
  Interface *interface = if_pair.first;
  InterfaceData *if_data = if_pair.second;
  std::thread emulation_thread(emulator, interface, &game_rom, false);
  emulation_thread.detach();
  interfaceLoop(if_data, emulation_thread);
}