#include "emulator/cpu/cpu.h"
#include "emulator/instructions/instruction.h"
#include "emulator/multimedia/graphics.h"
#include "emulator/utils/debuginstr.h"



void execute (State *state, Interface *interface, Short breakpoint)
{
  ulong n_instrs = 0;

  while (not state->stopped and
         state->PC != breakpoint and
         not state->config.end_emulation) {
    if (not state->halted) {
      Byte opcode = state->memory[state->PC];
      Byte data0 = state->memory[(state->PC+1)&0xFFFF];
      Byte data1 = state->memory[(state->PC+2)&0xFFFF];
      if (state->config.debug) {
        interface->print(cycleStr(opcode, data0, data1, state) + "\n");
      }
      state->PC += instrLen(opcode);
      state->cycles += executeInstruction(opcode, data0, data1, state);
    } else {
      state->cycles += 4; // Make clock work when halted
    }
    updateTimeRegisters(state);
    // Update buttons once normally and hang looking for button input if stopped
    do {
      updateButtons(n_instrs, state, interface);
      if (state->stopped) {
        interface->sleepMillis(10);
      }
    } while (state->stopped);
    updateGraphics(state, interface);
    checkAndCallInterrupt(state);
    state->config.end_emulation = interface->endEmulation();
    synchExecution(state, interface);
    n_instrs++;
  }
}