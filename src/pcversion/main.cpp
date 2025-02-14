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
  bool debug = ((argc > 1 and std::string(argv[1]) == "-d") or
                (argc > 2 and std::string(argv[2]) == "-d"));
  if (debug) {
    std::cout << std::endl << "(Executing in debug mode)" << std::endl;
  }
  std::pair<Interface*,InterfaceData*> if_pair = getInterface();
  Interface *interface = if_pair.first;
  InterfaceData *if_data = if_pair.second;
  std::thread emulation_thread(emulator, interface, &game_rom, debug);
  emulation_thread.detach();
  interfaceLoop(if_data, emulation_thread);
}