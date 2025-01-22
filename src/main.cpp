#include <Arduino.h>
#include "emulator/instructions/instruction.h"
#include "emulator/state.h"
#include "emulator/cpu/cpu.h"
#include "emulator/utils/debug.h"


// #define TESTING

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

void benchmarkAll (State *state)
{
  Serial.println("\nBegan testing!");
  for (int opcode = 0; opcode <= 0xFF; opcode++) {
    auto t_ini = micros();
    for (int i = 0; i < int(1e4); i++) {
      executeInstruction(opcode, 0x00, 0x00, state);
    }
    auto t_fi = micros();
    float ratio = (t_fi-t_ini)/1e4;
    String base_msg = " * " + String(opcode) + ": " +  String(ratio) + " s";
    if (ratio > 0.7) {
      Serial.println("WARNING: " + base_msg);
    } else {
      Serial.println(base_msg);
    }
    
  }
    for (int opcode = 0; opcode <= 0xFF; opcode++) {
    auto t_ini = micros();
    for (int i = 0; i < int(1e4); i++) {
      executeInstruction(0xCB, opcode, 0x00, state);
    }
    auto t_fi = micros();
    float ratio = (t_fi-t_ini)/1e4;
    String base_msg = " * (CB) " + String(opcode) + ": " +  String(ratio) + " s";
    if (ratio > 0.7) {
      Serial.println("WARNING: " + base_msg);
    } else {
      Serial.println(base_msg);
    }
    
  }
}

void execute (State *state)
{
  state->A = 0x12;
  state->B = 0x35;
  executeInstruction(0x80, 0xFF, 0xFF, state);
}

void testExecBlock (State *state)
{
  std::pair<int, bool> exec_result;
  initializeState(state);
  Serial.println("Beginning execution...");
  int total_cycles = 0;
  auto t_ini = micros();
  bool keep = true;
  while (keep) {
    exec_result = executeInstrBlock(state);
    total_cycles += exec_result.first;
    keep = exec_result.second;
    Serial.println("PC at: " + formatShort(state->PC));
  }
  auto t_fi = micros();
  float cycles = total_cycles;
  float t_theo = cycles*1e6/CLOCK_FREQ;
  Serial.println("Finished! Took: " + String(t_fi - t_ini) + " us / " + String(t_theo) + " us.");
  Serial.println("State:");
  showRegisters(state);
  showMemoryRange(state, 0xFF05, 0xFFFF);
}

void loop() {
  Serial.println("\nBegan testing!");
  State *state = new State;
  // benchmark(0xF8, 0x01, 0x00, state);
  // execute(state);
  // benchmarkAll(state);
  testExecBlock(state);
  delete state;
  delay(1e4);
}

#endif