#include <unity.h>
#include "arithmeticinstr.h"
#include "emulator/state.h"
#include "emulator/instructions/instruction.h"

void runAllArithmeticInstrTests ()
{
  RUN_TEST(test_instr_ADX_A_r_instr_ADX_A_mem_HL);
  RUN_TEST(test_instr_SBX_A_r_instr_SBX_A_mem_HL);
  RUN_TEST(test_instr_AND_OR_XOR_A_r);
  RUN_TEST(test_instr_CP_r);
  RUN_TEST(test_instr_INC_DEC);
  RUN_TEST(test_instr_ADD_16_bits);
  RUN_TEST(test_instr_ADD_SP_n);
  RUN_TEST(test_instr_INC_DEC_16_bits);
}

void test_instr_ADX_A_r_instr_ADX_A_mem_HL ()
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

  // Test 0x8C: ADC A, H (simple)
  state->A = 0x10;
  state->H = 0x23;
  state->F = 0xF0;
  cycles = executeInstruction(0x8C, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x34, state->A);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0x8E: ADC A, (HL) (simple)
  state->A = 0x10;
  state->H = 0x23;
  state->L = 0x53;
  state->memory[0x2353] = 0x07;
  state->F = 0xF0;
  cycles = executeInstruction(0x8E, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x18, state->A);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  delete state;
}

void test_instr_SBX_A_r_instr_SBX_A_mem_HL ()
{
  State *state = new State;
  int cycles;

  // Test 0xD6: SUB A, n (simple)
  state->A = 0x15;
  state->F = 0xF0;
  cycles = executeInstruction(0xD6, 0x11, 0xFF, state);
  TEST_ASSERT_EQUAL(0x04, state->A);
  TEST_ASSERT_EQUAL(SUBTRACT_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0x94: SUB A, H (half carry)
  state->A = 0x15;
  state->H = 0x06;
  state->F = 0xF0;
  cycles = executeInstruction(0x94, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x0F, state->A);
  TEST_ASSERT_EQUAL(SUBTRACT_FLAG | HALF_CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0x96: SUB A, (HL) (carry)
  state->A = 0x15;
  state->H = 0x06;
  state->L = 0x83;
  state->memory[0x0683] = 0x25;
  state->F = 0xF0;
  cycles = executeInstruction(0x96, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0xF0, state->A);
  TEST_ASSERT_EQUAL(SUBTRACT_FLAG | CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0x99: SBC A, C (simple)
  state->A = 0x15;
  state->C = 0x04;
  state->F = 0xF0;
  cycles = executeInstruction(0x99, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x10, state->A);
  TEST_ASSERT_EQUAL(SUBTRACT_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);
  delete state;
}

void test_instr_AND_OR_XOR_A_r ()
{
  State *state = new State;
  int cycles;

  // Test 0xA2: AND D
  state->A = 0x15;
  state->D = 0x32;
  state->F = 0xF0;
  cycles = executeInstruction(0xA2, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x10, state->A);
  TEST_ASSERT_EQUAL(HALF_CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0xF6: OR n
  state->A = 0x15;
  state->F = 0xF0;
  cycles = executeInstruction(0xF6, 0x22, 0xFF, state);
  TEST_ASSERT_EQUAL(0x37, state->A);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0xAE: XOR (HL) (zero)
  state->A = 0x15;
  state->H = 0x12;
  state->L = 0x34;
  state->memory[0x1234] = 0x15;
  state->F = 0xF0;
  cycles = executeInstruction(0xAE, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x00, state->A);
  TEST_ASSERT_EQUAL(ZERO_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  delete state;
}

void test_instr_CP_r ()
{
  State *state = new State;
  int cycles;

  // Test 0xFE: CP n (simple)
  state->A = 0x15;
  state->F = 0xF0;
  cycles = executeInstruction(0xFE, 0x11, 0xFF, state);
  TEST_ASSERT_EQUAL(0x15, state->A);
  TEST_ASSERT_EQUAL(SUBTRACT_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0xBC: CP H (half carry)
  state->A = 0x15;
  state->H = 0x06;
  state->F = 0xF0;
  cycles = executeInstruction(0xBC, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(SUBTRACT_FLAG | HALF_CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0xBE: CP (HL) (carry)
  state->A = 0x15;
  state->H = 0x06;
  state->L = 0x83;
  state->memory[0x0683] = 0x25;
  state->F = 0xF0;
  cycles = executeInstruction(0xBE, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(SUBTRACT_FLAG | CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  delete state;
}

void test_instr_INC_DEC ()
{
  State *state = new State;
  int cycles;

  // Test 0x3C: INC A (normal)
  state->A = 0x23;
  state->F = 0xF0;
  cycles = executeInstruction(0x3C, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x24, state->A);
  TEST_ASSERT_EQUAL(CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0x34: INC (HL) (zero and half carry)
  state->H = 0x06;
  state->L = 0x83;
  state->memory[0x0683] = 0xFF;
  state->F = 0xF0;
  cycles = executeInstruction(0x34, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x00, state->memory[0x0683]);
  TEST_ASSERT_EQUAL(CARRY_FLAG | HALF_CARRY_FLAG | ZERO_FLAG, state->F);
  TEST_ASSERT_EQUAL(12,    cycles);

  // Test 0x1D: DEC E (half carry)
  state->E = 0x20;
  state->F = 0xF0;
  cycles = executeInstruction(0x1D, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x1F, state->E);
  TEST_ASSERT_EQUAL(CARRY_FLAG | HALF_CARRY_FLAG | SUBTRACT_FLAG, state->F);
  TEST_ASSERT_EQUAL(4,    cycles);

  // Test 0x3%: DEC (HL) (zero and half carry)
  state->H = 0x06;
  state->L = 0x83;
  state->memory[0x0683] = 0x01;
  state->F = 0xF0;
  cycles = executeInstruction(0x35, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x00, state->memory[0x0683]);
  TEST_ASSERT_EQUAL(CARRY_FLAG | ZERO_FLAG | SUBTRACT_FLAG, state->F);
  TEST_ASSERT_EQUAL(12,    cycles);

  delete state;
}

void test_instr_ADD_16_bits ()
{
  State *state = new State;
  int cycles;

  // Test 0x09: ADD HL, BC (normal)
  state->H = 0x23;
  state->L = 0x62;
  state->B = 0x82;
  state->C = 0x12;
  state->F = 0xF0;
  cycles = executeInstruction(0x09, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0xA5, state->H);
  TEST_ASSERT_EQUAL(0x74, state->L);
  TEST_ASSERT_EQUAL(ZERO_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0x39: ADD HL, SP (carry and half carry)
  state->H  = 0xFF;
  state->L  = 0xFF;
  state->SP = 0x0001;
  state->F  = 0xF0;
  cycles = executeInstruction(0x39, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x00, state->H);
  TEST_ASSERT_EQUAL(0x00, state->L);
  TEST_ASSERT_EQUAL(ZERO_FLAG | HALF_CARRY_FLAG | CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(8,    cycles);

  delete state;
}

void test_instr_ADD_SP_n ()
{
  State *state = new State;
  int cycles;

  // Test 0xE8: ADD SP, n (normal)
  state->SP = 0x5839;
  state->F  = 0xF0;
  cycles = executeInstruction(0xE8, 0x05, 0xFF, state);
  TEST_ASSERT_EQUAL(0x583E, state->SP);
  TEST_ASSERT_EQUAL(0x00, state->F);
  TEST_ASSERT_EQUAL(16,    cycles);

  // Test 0xE8: ADD SP, n (negative n, carry and half carry)
  state->SP = 0x0000;
  state->F  = 0xF0;
  cycles = executeInstruction(0xE8, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0xFFFF, state->SP);
  TEST_ASSERT_EQUAL(CARRY_FLAG | HALF_CARRY_FLAG, state->F);
  TEST_ASSERT_EQUAL(16,    cycles);

  delete state;
}

void test_instr_INC_DEC_16_bits ()
{
  State *state = new State;
  int cycles;

  // Test 0x23: INC HL
  state->H = 0x12;
  state->L = 0x34;
  cycles = executeInstruction(0x23, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x12, state->H);
  TEST_ASSERT_EQUAL(0x35, state->L);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0x33: INC SP
  state->SP = 0x1234;
  cycles = executeInstruction(0x33, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x1235, state->SP);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0x1B: DEC DE
  state->D = 0x12;
  state->E = 0x34;
  cycles = executeInstruction(0x1B, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x12, state->D);
  TEST_ASSERT_EQUAL(0x33, state->E);
  TEST_ASSERT_EQUAL(8,    cycles);

  // Test 0x3B: DEC SP
  state->SP = 0x1234;
  cycles = executeInstruction(0x3B, 0xFF, 0xFF, state);
  TEST_ASSERT_EQUAL(0x1233, state->SP);
  TEST_ASSERT_EQUAL(8,    cycles);

  delete state;
}