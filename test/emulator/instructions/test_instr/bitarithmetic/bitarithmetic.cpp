#include <unity.h>
#include "bitarithmetic.h"
#include "emulator/state.h"
#include "emulator/instructions/instruction.h"

void runAllBitArithmeticInstrTests ()
{
  RUN_TEST(test_instr_SWAP);
}

void test_instr_SWAP ()
{
  State *state = new State;
  int cycles;

  // Test 0xCB 34: SWAP H
  state->H = 0x82;
  state->F = 0xF0;
  cycles = executeInstruction(0xCB, 0x34, 0xFF, state);
  TEST_ASSERT_EQUAL(0x28, state->H);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0xCB 37: SWAP A (zero)
  state->A = 0x00;
  state->F = 0xF0;
  cycles = executeInstruction(0xCB, 0x37, 0xFF, state);
  TEST_ASSERT_EQUAL(0x00, state->A);
  TEST_ASSERT_EQUAL(ZERO_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0xCB 36: SWAP (HL)
  state->H = 0x42;
  state->L = 0x76;
  state->memory[0x4276] = 0x37;
  state->F = 0xF0;
  cycles = executeInstruction(0xCB, 0x36, 0xFF, state);
  TEST_ASSERT_EQUAL(0x73, state->memory[0x4276]);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(16,    cycles);

  delete state;
}