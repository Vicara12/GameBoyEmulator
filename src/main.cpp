#include <Arduino.h>
#include "emulator/instructions/instruction.h"

// #include "instructions/loadinstr.h"
#include "emulator/state.h"

#define TESTING

#ifndef TESTING
#define

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("\nBegan testing!");
  State *state = new State;
  int total = 0;
  auto t_ini = micros();
  for (int i = 0; i < int(1e6); i++) {
    total += executeInstruction(0xF8, 0x01, 0x00, state);
    // total += instr_LDX_A_mem_HL(state, true, true);
  }
  auto t_fi = micros();
  Serial.println("Took " +  String((t_fi-t_ini)/1e6) + " secs " + String(total) + ".");
  delete state;
  delay(1e4);
}

#endif