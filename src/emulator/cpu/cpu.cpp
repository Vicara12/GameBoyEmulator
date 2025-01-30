#include "emulator/cpu/cpu.h"
#include "emulator/cpu/bootroom.h"
#include "emulator/instructions/instruction.h"
#include "emulator/multimedia/graphics.h"




// Execute until STOP or HALT found
void execute (State *state, Interface *interface, Short breakpoint)
{
  ulong n_instrs = 0;

  while (not (state->halted or state->stopped) and state->PC != breakpoint) {
    Byte opcode = state->memory[state->PC];
    Byte data0 = state->memory[(state->PC+1)&0xFFFF];
    Byte data1 = state->memory[(state->PC+2)&0xFFFF];
    state->PC += instrLen(opcode);
    state->cycles += executeInstruction(opcode, data0, data1, state);
    updateTimeRegisters(state);
    updateButtons(n_instrs, state, interface);
    updateGraphics(state, interface);
    if (state->ime) {
      checkAndCallInterrupt(state);
    }

    n_instrs++;
  }
}