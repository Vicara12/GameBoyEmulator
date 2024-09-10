#include <Arduino.h>
#include "instructions/instruction.h"

// #include "instructions/loadinstr.h"
#include "state.h"

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("\nBegan testing!");
  // Im using a pointer here becuase creating an object of type state here causes a stack overflow
  // when calling executeInstrcution because of some reason
  State *state = new State;
  int total = 0;
  auto t_ini = micros();
  for (int i = 0; i < int(1e6); i++) {
    total += executeInstruction(0xF8, 0x01, 0x00, *state);
    // total += instr_LDX_A_mem_HL(state, true, true);
  }
  auto t_fi = micros();
  Serial.println("Took " +  String((t_fi-t_ini)/1e6) + " secs " + String(total) + ".");
  delete state;
  delay(1e4);
}