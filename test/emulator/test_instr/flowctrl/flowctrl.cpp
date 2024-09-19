#include <unity.h>
#include "flowctrl.h"
#include "emulator/state.h"
#include "emulator/instructions/instruction.h"


void runAllFlowCtrlInstrTests ()
{
  RUN_TEST(test_instr_JP_nn_JP_HL_JR_n);
  RUN_TEST(test_instr_JP_cc_nn_JP_cc_n);
  RUN_TEST(test_instr_CALL_nn_CALL_cc_nn);
  RUN_TEST(test_instr_RST_n_RET_RET_cc);
}


void test_instr_JP_nn_JP_HL_JR_n ()
{
  State *state = new State;
  int cycles;

  // Test 0xC3: JP nn
  cycles = executeInstruction(0xC3, 0x12, 0x23, state);
  TEST_ASSERT_EQUAL(0x2312, state->PC);
  TEST_ASSERT_EQUAL(    12, cycles);

  // Test 0xE9: JP (HL)
  state->H = 0x73;
  state->L = 0x37;
  cycles = executeInstruction(0xE9, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x7337, state->PC);
  TEST_ASSERT_EQUAL(     4, cycles);

  // Test 0x18: JR n
  state->PC = 0x1234;
  cycles = executeInstruction(0x18, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x1233, state->PC);
  TEST_ASSERT_EQUAL(     8, cycles);

  delete state;
}

void test_instr_JP_cc_nn_JP_cc_n ()
{
  State *state = new State;
  int cycles;

  // Test 0xC2: JP NZ, nn (jump)
  state->F = SUBTRACT_FLAG | HALF_CARRY_FLAG | CARRY_FLAG;
  cycles = executeInstruction(0xC3, 0x12, 0x23, state);
  TEST_ASSERT_EQUAL(0x2312, state->PC);
  TEST_ASSERT_EQUAL(    12, cycles);

  // Test 0xCA: JP Z, nn (no jump)
  state->F = SUBTRACT_FLAG | HALF_CARRY_FLAG | CARRY_FLAG;
  cycles = executeInstruction(0xCA, 0x12, 0x24, state);
  TEST_ASSERT_EQUAL(0x2312, state->PC);
  TEST_ASSERT_EQUAL(    12, cycles);

  // Test 0x30: JR NC, n (no jump)
  state->PC = 0x6543;
  state->F = SUBTRACT_FLAG | HALF_CARRY_FLAG | CARRY_FLAG;
  cycles = executeInstruction(0x30, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x6543, state->PC);
  TEST_ASSERT_EQUAL(    8, cycles);

  // Test 0x38: JR C, nn (jump)
  state->PC = 0x6543;
  state->F = SUBTRACT_FLAG | HALF_CARRY_FLAG | CARRY_FLAG;
  cycles = executeInstruction(0x38, 0xFE, 0x24, state);
  TEST_ASSERT_EQUAL(0x6541, state->PC);
  TEST_ASSERT_EQUAL(    8, cycles);

  delete state;
}

void test_instr_CALL_nn_CALL_cc_nn ()
{
  State *state = new State;
  int cycles;

  // Test 0xCD: CALL nn
  state->PC = 0x1234;
  state->SP = 0x1000;
  cycles = executeInstruction(0xCD, 0xFE, 0x24, state);
  TEST_ASSERT_EQUAL(0x24FE, state->PC);
  TEST_ASSERT_EQUAL(0x0FFE, state->SP);
  TEST_ASSERT_EQUAL(0x12, state->memory[0x0FFF]);
  TEST_ASSERT_EQUAL(0x34, state->memory[0x0FFE]);
  TEST_ASSERT_EQUAL(  12, cycles);

  // Test 0xCC: CALL Z,nn (call)
  state->PC = 0x1234;
  state->SP = 0x1010;
  state->F = ZERO_FLAG;
  cycles = executeInstruction(0xCC, 0xFE, 0x24, state);
  TEST_ASSERT_EQUAL(0x24FE, state->PC);
  TEST_ASSERT_EQUAL(0x100E, state->SP);
  TEST_ASSERT_EQUAL(0x12, state->memory[0x100F]);
  TEST_ASSERT_EQUAL(0x34, state->memory[0x100E]);
  TEST_ASSERT_EQUAL(  12, cycles);

  // Test 0xD4: CALL NC, nn (no call)
  state->PC = 0x1230;
  state->F = CARRY_FLAG;
  cycles = executeInstruction(0xD4, 0xFE, 0x24, state);
  TEST_ASSERT_EQUAL(0x1230, state->PC);

  delete state;
}

void test_instr_RST_n_RET_RET_cc ()
{
  State *state = new State;
  int cycles;

  // Test 0xEF: RST 28H
  state->PC = 0x1234;
  state->SP = 0x1000;
  cycles = executeInstruction(0xEF, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x0028, state->PC);
  TEST_ASSERT_EQUAL(0x0FFE, state->SP);
  TEST_ASSERT_EQUAL(0x12, state->memory[0x0FFF]);
  TEST_ASSERT_EQUAL(0x34, state->memory[0x0FFE]);
  TEST_ASSERT_EQUAL(  32, cycles);

  // Test 0xC9: RET
  state->SP = 0x1010;
  state->memory[0x1010] = 0x89;
  state->memory[0x1011] = 0x67;
  cycles = executeInstruction(0xC9, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x6789, state->PC);
  TEST_ASSERT_EQUAL(0x1012, state->SP);
  TEST_ASSERT_EQUAL(     8, cycles);

  // Test 0xC8: RET Z
  state->PC = 0x1234;
  state->SP = 0x1000;
  state->memory[0x1000] = 0x28;
  state->memory[0x1001] = 0x11;
  state->F = ZERO_FLAG;
  cycles = executeInstruction(0xC8, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x1128, state->PC);
  TEST_ASSERT_EQUAL(0x1002, state->SP);
  TEST_ASSERT_EQUAL(0x12, state->memory[0x0FFF]);
  TEST_ASSERT_EQUAL(0x34, state->memory[0x0FFE]);
  TEST_ASSERT_EQUAL(  8, cycles);

  delete state;
}