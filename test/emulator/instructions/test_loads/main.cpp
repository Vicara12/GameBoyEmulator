#include <unity.h>
#include <Arduino.h>
#include "emulator/state.h"
#include "emulator/instructions/instruction.h"

void test_instr_LD_nn_n ()
{
  State *state = new State;
  int cycles;

  // Test 0x26: LD H, 0x06
  cycles = executeInstruction(0x26, 0x06, 0xFF, state);
  TEST_ASSERT_EQUAL(state->H, 0x06);
  TEST_ASSERT_EQUAL(cycles, 8);

  // Test 0x2E: LD L, 0x07
  cycles = executeInstruction(0x2E, 0x07, 0xFF, state);
  TEST_ASSERT_EQUAL(state->L, 0x07);
  TEST_ASSERT_EQUAL(cycles, 8);

  delete state;
}

void test_instr_LD_r1_r2 ()
{
  State *state = new State;
  int cycles;

  // Test 0x7F: LD A, A
  state->A = 0x12;
  cycles = executeInstruction(0x7F, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(state->A, 0x12);
  TEST_ASSERT_EQUAL(cycles, 4);

  // Test 0x78: LD A, B
  state->A = 0x21;
  state->B = 0x12;
  cycles = executeInstruction(0x78, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(state->A, 0x12);
  TEST_ASSERT_EQUAL(cycles, 4);

  delete state;
}

void test_instr_LD_r1_mem_nn ()
{
  State *state = new State;
  int cycles;

  // Test 0xFA: LD A, (nn)
  state->A = 0x21;
  state->memory[0x1234] = 0x12;
  cycles = executeInstruction(0xFA, 0x34, 0x12, state);
  TEST_ASSERT_EQUAL(state->A, 0x12);
  TEST_ASSERT_EQUAL(cycles, 16);

  // Test 0x0A: LD A, (BC)
  state->A = 0x21;
  state->B = 0x43;
  state->C = 0x21;
  state->memory[0x4321] = 0x42;
  cycles = executeInstruction(0x0A, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(state->A, 0x42);
  TEST_ASSERT_EQUAL(cycles, 8);

  delete state;
}

void test_instr_LD_mem_r1_nn ()
{
  State *state = new State;
  int cycles;

  // Test 0x12: LD (DE), A
  state->A = 0x21;
  state->B = 0x98;
  state->C = 0x76;
  state->memory[0x9876] = 0x37;
  cycles = executeInstruction(0x0A, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(state->A, 0x37);
  TEST_ASSERT_EQUAL(cycles, 8);

  // Test 0xEA: LD (nn), A
  state->A = 0x21;
  state->memory[0x0137] = 0x48;
  cycles = executeInstruction(0xEA, 0x37, 0x01, state);
  TEST_ASSERT_EQUAL(state->A, 0x21);
  TEST_ASSERT_EQUAL(cycles, 16);

  // Test 0x36: LD (HL), 0x73
  state->H = 0x36;
  state->L = 0x95;
  cycles = executeInstruction(0x36, 0x73, 0xFF, state);
  TEST_ASSERT_EQUAL(state->memory[0x3695], 0x73);
  TEST_ASSERT_EQUAL(cycles, 12);

  delete state;
}

void test_instr_LD_A_FF00_n ()
{
  State *state = new State;
  int cycles;

  // Test 0xF0: LD A, (0xFF00 + n)
  state->A = 0x01;
  state->memory[0xFF25] = 0x30;
  cycles = executeInstruction(0xF0, 0x25, 0xFF, state);
  TEST_ASSERT_EQUAL(state->A, 0x30);
  TEST_ASSERT_EQUAL(cycles, 12);

  // Test 0xF2: LD A, (0xFF00 + C)
  state->A = 0x01;
  state->C = 0x84;
  state->memory[0xFF84] = 0x32;
  cycles = executeInstruction(0xF2, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(state->A, 0x32);
  TEST_ASSERT_EQUAL(cycles, 8);

  delete state;
}

void test_instr_LD_FF00_n_A ()
{
  State *state = new State;
  int cycles;

  // Test 0xE0: LD (0xFF00 + n), A 
  state->A = 0x01;
  state->memory[0xFF25] = 0x30;
  cycles = executeInstruction(0xE0, 0x25, 0xFF, state);
  TEST_ASSERT_EQUAL(state->memory[0xFF25], 0x01);
  TEST_ASSERT_EQUAL(cycles, 12);

  // Test 0xE2: LD (0xFF00 + C), A
  state->A = 0x02;
  state->C = 0x84;
  state->memory[0xFF84] = 0x32;
  cycles = executeInstruction(0xE2, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(state->memory[0xFF84], 0x02);
  TEST_ASSERT_EQUAL(cycles, 8);

  delete state;
}

void test_instr_LDX_A_mem_HL ()
{
  State *state = new State;
  int cycles;

  // Test 0x22: LDI (HL), A
  state->A = 0x01;
  state->H = 0x12;
  state->L = 0x34;
  state->memory[0x1234] = 0x34;
  cycles = executeInstruction(0x22, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(state->memory[0x1234], 0x01);
  TEST_ASSERT_EQUAL(state->L, 0x35);
  TEST_ASSERT_EQUAL(cycles, 8);

  // Test 0x32: LDD (HL), A
  state->A = 0x02;
  state->H = 0x12;
  state->L = 0x34;
  state->memory[0x1234] = 0x34;
  cycles = executeInstruction(0x32, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(state->memory[0x1234], 0x02);
  TEST_ASSERT_EQUAL(state->L, 0x33);
  TEST_ASSERT_EQUAL(cycles, 8);

  // Test 0x2A: LDI A, (HL)
  state->A = 0x01;
  state->H = 0x12;
  state->L = 0x34;
  state->memory[0x1234] = 0x34;
  cycles = executeInstruction(0x2A, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(state->A, 0x34);
  TEST_ASSERT_EQUAL(state->L, 0x35);
  TEST_ASSERT_EQUAL(cycles, 8);

  // Test 0x3A: LDD A, (HL)
  state->A = 0x01;
  state->H = 0x12;
  state->L = 0x34;
  state->memory[0x1234] = 0x30;
  cycles = executeInstruction(0x3A, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(state->A, 0x30);
  TEST_ASSERT_EQUAL(state->L, 0x33);
  TEST_ASSERT_EQUAL(cycles, 8);

  delete state;
}

void test_instr_LD_dReg_nn_instr_LD_SP_nn ()
{
  State *state = new State;
  int cycles;

  // Test 0x01: LD BC, nn
  state->B = 0x12;
  state->C = 0x34;
  cycles = executeInstruction(0x01, 0x34, 0x21, state);
  TEST_ASSERT_EQUAL(state->B, 0x21);
  TEST_ASSERT_EQUAL(state->C, 0x34);
  TEST_ASSERT_EQUAL(cycles, 12);

  // Test 0x31: LD SP, nn
  cycles = executeInstruction(0x31, 0x28, 0x39, state);
  TEST_ASSERT_EQUAL(state->SP, 0x3928);
  TEST_ASSERT_EQUAL(cycles, 12);

  delete state;
}

void test_instr_LDHL_SP_n ()
{
  State *state = new State;
  int cycles;

  // Test 0xF8: LDHL SP, n (normal)
  state->SP = 0x1232;
  state->F = 0xF0;
  cycles = executeInstruction(0xF8, 0x02, 0x21, state);
  TEST_ASSERT_EQUAL(state->H, 0x12);
  TEST_ASSERT_EQUAL(state->L, 0x34);
  TEST_ASSERT_EQUAL(state->F, 0x00);
  TEST_ASSERT_EQUAL(cycles, 12);

  // Test 0xF8: LDHL SP, n (negative literal)
  state->SP = 0x1232;
  state->F = 0xF0;
  cycles = executeInstruction(0xF8, -2, 0x21, state);
  TEST_ASSERT_EQUAL(state->H, 0x12);
  TEST_ASSERT_EQUAL(state->L, 0x30);
  TEST_ASSERT_EQUAL(state->F, 0x00);
  TEST_ASSERT_EQUAL(cycles, 12);

  // Test 0xF8: LDHL SP, n (half carry)
  state->SP = 0x12FE;
  state->F = 0xF0;
  cycles = executeInstruction(0xF8, 0x04, 0x21, state);
  TEST_ASSERT_EQUAL(state->H, 0x13);
  TEST_ASSERT_EQUAL(state->L, 0x02);
  TEST_ASSERT_EQUAL(state->F, HALF_CARRY_FLAG);
  TEST_ASSERT_EQUAL(cycles, 12);

  // Test 0xF8: LDHL SP, n (carry)
  state->SP = 0xFFFE;
  state->F = 0xF0;
  cycles = executeInstruction(0xF8, 0x04, 0x21, state);
  TEST_ASSERT_EQUAL(state->H, 0x00);
  TEST_ASSERT_EQUAL(state->L, 0x02);
  TEST_ASSERT_EQUAL(state->F, CARRY_FLAG | HALF_CARRY_FLAG);
  TEST_ASSERT_EQUAL(cycles, 12);

  delete state;
}

void test_instr_LD_mem_nn_SP ()
{
  State *state = new State;
  int cycles;

  // Test 0x08: LD (nn), SP
  state->SP = 0x12FE;
  cycles = executeInstruction(0x08, 0x35, 0x39, state);
  TEST_ASSERT_EQUAL(state->memory[0x3935], 0xFE);
  TEST_ASSERT_EQUAL(state->memory[0x3936], 0x12);
  TEST_ASSERT_EQUAL(cycles, 20);

  delete state;
}

void test_instr_PUSH_nn ()
{
  State *state = new State;
  int cycles;

  // Test 0xC5: PUSH BC
  state->SP = 0x5829;
  state->B = 0x48;
  state->C = 0x26;
  cycles = executeInstruction(0xC5, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(state->memory[0x5829], 0x48);
  TEST_ASSERT_EQUAL(state->memory[0x5828], 0x26);
  TEST_ASSERT_EQUAL(state->SP, 0x5827);
  TEST_ASSERT_EQUAL(cycles, 16);

  delete state;
}

void test_instr_POP_nn ()
{
  State *state = new State;
  int cycles;

  // Test 0xD1: POP DE
  state->SP = 0x0372;
  state->memory[0x0372] = 0x83;
  state->memory[0x0373] = 0x94;
  cycles = executeInstruction(0xD1, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(state->D, 0x94);
  TEST_ASSERT_EQUAL(state->E, 0x83);
  TEST_ASSERT_EQUAL(state->SP, 0x0374);
  TEST_ASSERT_EQUAL(cycles, 12);

  delete state;
}

void setup ()
{
  UNITY_BEGIN();
  RUN_TEST(test_instr_LD_nn_n);
  RUN_TEST(test_instr_LD_r1_r2);
  RUN_TEST(test_instr_LD_r1_mem_nn);
  RUN_TEST(test_instr_LD_mem_r1_nn);
  RUN_TEST(test_instr_LD_A_FF00_n);
  RUN_TEST(test_instr_LD_FF00_n_A);
  RUN_TEST(test_instr_LDX_A_mem_HL);
  RUN_TEST(test_instr_LD_dReg_nn_instr_LD_SP_nn);
  RUN_TEST(test_instr_LDHL_SP_n);
  RUN_TEST(test_instr_LD_mem_nn_SP);
  RUN_TEST(test_instr_PUSH_nn);
  RUN_TEST(test_instr_POP_nn);
  UNITY_END();
}

void loop ()
{
  delay(100);
}