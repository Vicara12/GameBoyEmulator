#include <unity.h>
#include "misc.h"
#include "emulator/state.h"
#include "emulator/instructions/instruction.h"


void runAllMiscInstrTests ()
{
  RUN_TEST(test_instr_DAA);
}

void test_instr_DAA ()
{
  State *state = new State;
  int cycles;

  // Test 0x27: DAA A (basic)
  state->A = 0x77;
  state->F = 0x00;
  cycles = executeInstruction(0x27, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x77, state->A);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0x27: DAA A (offset lower nibble)
  state->A = 0x6B;
  state->F = 0x00;
  cycles = executeInstruction(0x27, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x71, state->A);
  TEST_ASSERT_EQUAL(0x00, state->F);

  // Test 0x27: DAA A (offset upper nibble)
  state->A = 0xC4;
  state->F = 0x00;
  cycles = executeInstruction(0x27, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x24, state->A);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);

  // Test 0x27: DAA A (offset both nibbles)
  state->A = 0x9C;
  state->F = 0x00;
  cycles = executeInstruction(0x27, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x02, state->A);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);

  // Test 0x27: DAA A (carry on upper nibble)
  state->A = 0x10;
  state->F = CARRY_FLAG;
  cycles = executeInstruction(0x27, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x70, state->A);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);

  // Test 0x27: DAA A (carry on lower nibble)
  state->A = 0x11;
  state->F = HALF_CARRY_FLAG;
  cycles = executeInstruction(0x27, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x17, state->A);
  TEST_ASSERT_EQUAL(0x00, state->F);

  // Test 0x27: DAA A (basic subtraction)
  state->A = 0x77;
  state->F = SUBTRACT_FLAG;
  cycles = executeInstruction(0x27, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x77, state->A);
  TEST_ASSERT_EQUAL(SUBTRACT_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0x27: DAA A (subtraction offset lower nibble)
  state->A = 0x0D;
  state->F = SUBTRACT_FLAG | HALF_CARRY_FLAG;
  cycles = executeInstruction(0x27, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x07, state->A);
  TEST_ASSERT_EQUAL(SUBTRACT_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0x27: DAA A (subtraction half carry)
  state->A = 0xE4;
  state->F = SUBTRACT_FLAG | CARRY_FLAG;
  cycles = executeInstruction(0x27, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x84, state->A);
  TEST_ASSERT_EQUAL(SUBTRACT_FLAG | CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  delete state;
}