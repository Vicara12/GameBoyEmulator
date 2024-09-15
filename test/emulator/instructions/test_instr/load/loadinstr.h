#pragma once

void runAllLoadInstrTests ();

void test_instr_LD_nn_n ();

void test_instr_LD_r1_r2 ();

void test_instr_LD_r1_mem_nn ();

void test_instr_LD_mem_r1_nn ();

void test_instr_LD_A_FF00_n ();

void test_instr_LD_FF00_n_A ();

void test_instr_LDX_A_mem_HL ();

void test_instr_LD_dReg_nn_instr_LD_SP_nn ();

void test_instr_LDHL_SP_n ();

void test_instr_LD_mem_nn_SP ();

void test_instr_PUSH_nn ();

void test_instr_POP_nn ();