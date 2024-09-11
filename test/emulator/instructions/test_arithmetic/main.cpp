#include <unity.h>
#include <Arduino.h>
#include "emulator/state.h"
#include "emulator/instructions/instruction.h"

void test_instr_ADD_A_r_instr_ADD_A_mem_HL ()
{
  State *state = new State;
  int cycles;

  // Test 0x82: ADD A, D (simple)
  state->A = 0x10;
  state->D = 0x23;
  state->F = 0xF0;
  cycles = executeInstruction(0x82, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x33, state->A);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0x83: ADD A, E (half carry)
  state->A = 0x1F;
  state->E = 0x23;
  state->F = 0xF0;
  cycles = executeInstruction(0x83, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x42, state->A);
  TEST_ASSERT_EQUAL(HALF_CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0x84: ADD A, H (carry and zero)
  state->A = 0xF0;
  state->H = 0x10;
  state->F = 0xF0;
  cycles = executeInstruction(0x84, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x00, state->A);
  TEST_ASSERT_EQUAL(CARRY_FLAG | ZERO_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0x86: ADD A, (HL) (simple)
  state->A = 0x10;
  state->H = 0x23;
  state->L = 0x53;
  state->memory[0x2353] = 0x07;
  state->F = 0xF0;
  cycles = executeInstruction(0x86, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x17, state->A);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0xC6: ADD A, n (simple)
  state->A = 0x10;
  state->F = 0xF0;
  cycles = executeInstruction(0xC6, 0x41, 0xFF, state);
  TEST_ASSERT_EQUAL(0x51, state->A);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  delete state;
}

void setup ()
{
  UNITY_BEGIN();
  RUN_TEST(test_instr_ADD_A_r_instr_ADD_A_mem_HL);

  UNITY_END();
}

void loop ()
{
  delay(100);
}