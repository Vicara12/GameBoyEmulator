#include "emulator/cpu/cpu.h"
#include "emulator/cpu/bootroom.h"
#include "emulator/instructions/instruction.h"


void initializeState (State *state, Interface *interface)
{
  state->PC = 0x0000;
  loadBootRoom(state);
}

// Execute INSTR_BLOCK_N instructions. If execution flow is interrupted earlier,
// function returns automatically. Returns number of clock cycles executed and
// true if it finished the INSTR_BLOCK_N or false if it terminated from other source.
std::pair<int, bool> executeInstrBlock (State *state, Interface *interface)
{
  int n_instrs = 0;
  int total_cycles = 0;

  while (n_instrs < INSTR_BLOCK_N) {
    Byte opcode = state->memory[state->PC];
    Byte data0 = state->memory[(state->PC+1)&0xFFFF];
    Byte data1 = state->memory[(state->PC+2)&0xFFFF];
    if (state->PC >= 0x0100) {
      break;
    }
    state->PC += instrLen(opcode);
    total_cycles += executeInstruction(opcode, data0, data1, state, interface);
    if (state->halted or state->stopped) {
      break;
    }
    n_instrs++;
  }

  return std::make_pair(total_cycles, n_instrs == INSTR_BLOCK_N);
}