#include "emulator/utils/debug.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "emulator/utils/initialization.h"
#include "emulator/cpu/cpu.h"


std::string formatByte (Byte b)
{
  std::stringstream ss;
  // Convert b to short because for some reason it does not like uint8_t (Byte)
  ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << Short(b);
  return ss.str();
}


std::string formatShort (Short s)
{
  std::stringstream ss;
  ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << std::uppercase << s;
  return ss.str();
}


void showRegisters (State *state, Interface *interface)
{
  interface->print("A  = " + formatByte(state->A ) + "    F  = " + formatByte(state->F) + "\n");
  interface->print("B  = " + formatByte(state->B ) + "    C  = " + formatByte(state->C) + "\n");
  interface->print("D  = " + formatByte(state->D ) + "    E  = " + formatByte(state->E) + "\n");
  interface->print("H  = " + formatByte(state->H ) + "    L  = " + formatByte(state->L) + "\n");
  interface->print("SP = " + formatShort(state->SP)  + "  PC = " + formatShort(state->PC) + "\n");
}


void showMemoryRange (State *state, Short ini, Short fi, Interface *interface)
{
  for (int i = ini; i <= fi; i++) {
    interface->print("[" + formatShort(i) + "] = " + formatByte(state->memory[i]) + "\n");
  }
}


State* runInDebug (State *state, Interface *interface)
{
  int user_input = 0x0100; // Entry point to the rom

  while (user_input > -3) {
    interface->print("\n");
    execute(state, interface, Short(user_input));
    showRegisters(state, interface);
    interface->print("Enter next bp, -1 to show @ or -2 to show @ rang, -3 to toggle instr show [");
    interface->print(state->config.debug + "] or -4 to quit.");
    do {
      interface->print("\n - Input: ");
      user_input = interface->userHexInt();
      if (user_input == -1) {
        interface->print(" - Address: ");
        Short addr = interface->userHexInt();
        Byte res = state->memory[addr];
        interface->print("[" + formatShort(addr) + "] = " + formatByte(res));
      }
      else if (user_input == -2) {
        interface->print(" - From: ");
        Short addr_from = interface->userHexInt();
        interface->print(" - To:   ");
        Short addr_to = interface->userHexInt();
        showMemoryRange(state, addr_from, addr_to, interface);
      } else if (user_input == -3) {
        state->config.debug = not state->config.debug;
      }
    } while (user_input < 0 and user_input > -4);
  }

  return state;
}