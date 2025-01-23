#include "emulator/utils/debug.h"
#include <string>
#include <sstream>
#include <iomanip>

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