#include "emulator/utils/debug.h"

String formatByte (Byte b)
{
  if (b == 0) {
    return "0x00";
  }
  else if (b < 0x10) {
    return "0x0" + String(b,HEX);
  }
  return "0x" + String(b, HEX);
}

String formatShort (Short s)
{
  if (s == 0) {
    return "0x0000";
  }
  else if (s < 0x10) {
    return "0x000" + String(s, HEX);
  }
  else if (s < 0x100) {
    return "0x00" + String(s, HEX);
  }
  else if (s < 0x1000) {
    return "0x0" + String(s, HEX);
  }
  return "0x" +  String(s, HEX);
}

void showRegisters (State *state)
{
  Serial.println("A  = " + formatByte(state->A ) + "    F  = " + formatByte(state->F));
  Serial.println("B  = " + formatByte(state->B ) + "    C  = " + formatByte(state->C));
  Serial.println("D  = " + formatByte(state->D ) + "    E  = " + formatByte(state->E));
  Serial.println("H  = " + formatByte(state->H ) + "    L  = " + formatByte(state->L));
  Serial.println("SP = " + formatShort(state->SP)  + "  PC = " + formatShort(state->PC));
}

void showMemoryRange (State *state, Short ini, Short fi)
{
  for (int i = ini; i <= fi; i++) {
    Serial.println("[" + formatShort(i) + "] = " + formatByte(state->memory[i]));
  }
}