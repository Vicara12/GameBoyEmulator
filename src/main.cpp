#include <Arduino.h>
#include "emulator/instructions/instruction.h"
#include "emulator/state.h"


#define TESTING

#ifndef TESTING

void setup() {
  Serial.begin(9600);
}

void benchmark (Byte opcode, Byte data0, Byte data1, State *state)
{
  Serial.println("\nBegan testing!");
  int total = 0;
  auto t_ini = micros();
  for (int i = 0; i < int(1e6); i++) {
    total += executeInstruction(opcode, data0, data1, state);
    // total += instr_LDX_A_mem_HL(state, true, true);
  }
  auto t_fi = micros();
  Serial.println("Took " +  String((t_fi-t_ini)/1e6) + " secs " + String(total) + ".");
}

void execute (State *state)
{
  state->A = 0x12;
  state->B = 0x35;
  executeInstruction(0x80, 0xFF, 0xFF, state);
}

void loop() {
  Serial.println("\nBegan testing!");
  State *state = new State;
  // benchmark(0xF8, 0x01, 0x00, state);
  execute(state);
  delete state;
  delay(1e4);
}

#endif