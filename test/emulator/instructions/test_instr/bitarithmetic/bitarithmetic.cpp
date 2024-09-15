#include <unity.h>
#include "bitarithmetic.h"
#include "emulator/state.h"
#include "emulator/instructions/instruction.h"

void runAllBitArithmeticInstrTests ()
{
  RUN_TEST(test_instr_SWAP);
  RUN_TEST(test_instr_CPL);
  RUN_TEST(test_instr_CCF_SCF);
  RUN_TEST(test_RR_RRC_RL_RLC);
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

void test_instr_CPL ()
{
  State *state = new State;
  int cycles;

  // Test 0x2F: CPL
  state->A = 0x42;
  state->F = 0x00;
  cycles = executeInstruction(0x2F, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0xBD, state->A);
  TEST_ASSERT_EQUAL(SUBTRACT_FLAG | HALF_CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  delete state;
}

void test_instr_CCF_SCF ()
{
  State *state = new State;
  int cycles;

  // Test 0x3F: CCF
  state->F = 0x00;
  cycles = executeInstruction(0x3F, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0x37: SCF
  state->F = 0x00;
  cycles = executeInstruction(0x37, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  delete state;
}

void test_RR_RRC_RL_RLC ()
{
  State *state = new State;
  int cycles;

  // Test 0x07: RLCA
  state->A = 0x92;
  cycles = executeInstruction(0x07, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x24, state->A);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0xCB 16: RL (HL)
  state->H = 0x12;
  state->L = 0x34;
  state->memory[0x1234] = 0x92;
  state->F = 0xF0;
  cycles = executeInstruction(0xCB, 0x16, 0xFF, state);
  TEST_ASSERT_EQUAL(0x25, state->memory[0x1234]);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(16,    cycles);

  // Test 0xCB 0B: RRC E
  state->E = 0x95;
  cycles = executeInstruction(0xCB, 0x0B, 0xFF, state);
  TEST_ASSERT_EQUAL(0x4A, state->E);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0xCB 18: RR B
  state->B = 0x95;
  cycles = executeInstruction(0xCB, 0x18, 0xFF, state);
  TEST_ASSERT_EQUAL(0xCA, state->B);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  delete state;
}