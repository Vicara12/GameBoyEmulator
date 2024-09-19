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
  RUN_TEST(test_SLA_SRA_SRL);
  RUN_TEST(test_BIT_SET_RES);
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

void test_SLA_SRA_SRL ()
{
  State *state = new State;
  int cycles;

  // Test 0xCB 27: SLA A
  state->A = 0x93;
  state->F = 0x00;
  cycles = executeInstruction(0xCB, 0x27, 0xFF, state);
  TEST_ASSERT_EQUAL(0x26, state->A);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0xCB 26: SLA (HL)
  state->H = 0x72;
  state->L = 0x63;
  state->memory[0x7263] = 0x33;
  state->F = 0xF0;
  cycles = executeInstruction(0xCB, 0x26, 0xFF, state);
  TEST_ASSERT_EQUAL(0x66, state->memory[0x7263]);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(16,    cycles);

  // Test 0xCB 29: SRA C
  state->C = 0x93;
  state->F = 0x00;
  cycles = executeInstruction(0xCB, 0x29, 0xFF, state);
  TEST_ASSERT_EQUAL(0xC9, state->C);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0xCB 2E: SRA (HL)
  state->H = 0x62;
  state->L = 0x63;
  state->memory[0x6263] = 0x52;
  state->F = 0xF0;
  cycles = executeInstruction(0xCB, 0x2E, 0xFF, state);
  TEST_ASSERT_EQUAL(0x29, state->memory[0x6263]);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(16,    cycles);

  // Test 0xCB 3C: SRL H
  state->H = 0x93;
  state->F = 0x00;
  cycles = executeInstruction(0xCB, 0x3C, 0xFF, state);
  TEST_ASSERT_EQUAL(0x49, state->H);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0xCB 2E: SRL (HL)
  state->H = 0x52;
  state->L = 0x63;
  state->memory[0x5263] = 0x12;
  state->F = 0xF0;
  cycles = executeInstruction(0xCB, 0x2E, 0xFF, state);
  TEST_ASSERT_EQUAL(0x09, state->memory[0x5263]);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(16,    cycles);

  delete state;
}

void test_BIT_SET_RES ()
{
  State *state = new State;
  int cycles;

  // Test 0xCB 4F: BIT 1, A
  state->A = 0x93;
  state->F = CARRY_FLAG;
  cycles = executeInstruction(0xCB, 0x4F, 0xFF, state);
  TEST_ASSERT_EQUAL(0x93, state->A);
  TEST_ASSERT_EQUAL(ZERO_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0xCB 76: BIT 6, (HL)
  state->H = 0x72;
  state->L = 0x63;
  state->memory[0x7263] = 0x33;
  state->F = 0xF0;
  cycles = executeInstruction(0xCB, 0x76, 0xFF, state);
  TEST_ASSERT_EQUAL(0x33, state->memory[0x7263]);
  TEST_ASSERT_EQUAL(HALF_CARRY_FLAG | CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(16,    cycles);

  // Test 0xCB AB: RES 5, E
  state->E = 0x33;
  state->F = CARRY_FLAG;
  cycles = executeInstruction(0xCB, 0xAB, 0xFF, state);
  TEST_ASSERT_EQUAL(0x13, state->E);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0xCB 8E: RES 1, (HL)
  state->H = 0x72;
  state->L = 0x63;
  state->memory[0x7263] = 0x35;
  state->F = 0xF0;
  cycles = executeInstruction(0xCB, 0x8E, 0xFF, state);
  TEST_ASSERT_EQUAL(0x35, state->memory[0x7263]);
  TEST_ASSERT_EQUAL(0xF0, state->F);
  TEST_ASSERT_EQUAL(16,    cycles);

  // Test 0xCB CC: SET 1, H
  state->H = 0x91;
  state->F = CARRY_FLAG;
  cycles = executeInstruction(0xCB, 0xCC, 0xFF, state);
  TEST_ASSERT_EQUAL(0x93, state->A);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0xCB E6: SET 4, (HL)
  state->H = 0x72;
  state->L = 0x63;
  state->memory[0x7263] = 0x23;
  state->F = 0xF0;
  cycles = executeInstruction(0xCB, 0xE6, 0xFF, state);
  TEST_ASSERT_EQUAL(0x33, state->memory[0x7263]);
  TEST_ASSERT_EQUAL(0xF0, state->F);
  TEST_ASSERT_EQUAL(16,    cycles);

  delete state;
}