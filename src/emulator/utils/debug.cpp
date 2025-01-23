#include "emulator/utils/debug.h"
#include <string>
#include <sstream>
#include <iomanip>

std::string formatByte (Byte b)
{
  std::stringstream ss;
  ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << b;
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
  interface->print("A  = " + formatByte(state->A ) + "    F  = " + formatByte(state->F));
  interface->print("B  = " + formatByte(state->B ) + "    C  = " + formatByte(state->C));
  interface->print("D  = " + formatByte(state->D ) + "    E  = " + formatByte(state->E));
  interface->print("H  = " + formatByte(state->H ) + "    L  = " + formatByte(state->L));
  interface->print("SP = " + formatShort(state->SP)  + "  PC = " + formatShort(state->PC));
}

void showMemoryRange (State *state, Short ini, Short fi, Interface *interface)
{
  for (int i = ini; i <= fi; i++) {
    interface->print("[" + formatShort(i) + "] = " + formatByte(state->memory[i]));
  }
}