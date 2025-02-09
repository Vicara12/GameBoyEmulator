#include <SFML/Graphics.hpp>
#include <thread>
#include "pcversion/interfaceadapter.h"
#include "pcversion/program.h"
#include "emulator/emulator.h"
#include "tetris.h"



int main ()
{
  std::pair<Interface*,InterfaceData*> if_pair = getInterface();
  Interface *interface = if_pair.first;
  InterfaceData *if_data = if_pair.second;
  std::thread emulation_thread(emulator, interface, &tetris_rom);
  emulation_thread.detach();
  interfaceLoop(if_data, emulation_thread);
}