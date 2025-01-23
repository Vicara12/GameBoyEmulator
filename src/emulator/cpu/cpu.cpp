#include "emulator/cpu/cpu.h"
#include "emulator/cpu/bootroom.h"
#include "emulator/instructions/instruction.h"


void initializeState (State *state)
{
  // Initialize registers and boot room
  state->PC = 0x0000;
  loadBootRoom(state);
}

// Execute until STOP or HALT found
void execute (State *state, Interface *interface)
{
  ulong n_instrs = 0;

  while (not (state->halted or state->stopped)) {
    Byte opcode = state->memory[state->PC];
    Byte data0 = state->memory[(state->PC+1)&0xFFFF];
    Byte data1 = state->memory[(state->PC+2)&0xFFFF];
    if (state->PC >= 0x0100) { // DEBUGCODE
      break;
    }
    state->PC += instrLen(opcode);
    state->cycles += executeInstruction(opcode, data0, data1, state);

    // Update buttons pressed each INSTRS_PER_BUTTON_UPDATE instructions
    if (n_instrs%INSTRS_PER_BUTTON_UPDATE == 0) {
      Byte old_buttons_pressed = state->buttons_pressed;
      state->buttons_pressed = interface->readButtons();
      // If any change in button pressed activate interrupt
      if (old_buttons_pressed ^ state->buttons_pressed != 0) {
        // TODO: button interrupt
      }
    }

    // Update time registers
    state->memory[DIV_REGISTER] = (state->cycles - state->cycles_last_DIV)/256;
    // TODO: reset this register when the STOP mode ends
    if (state->enable_TIMA) {
      Byte old_TIMA = state->memory[TIMA_REGISTER];
      state->memory[TIMA_REGISTER] = (state->cycles - state->cycles_last_TIMA)/state->cycles_div_TIMA;
      // Detect TIMA overflow
      if (old_TIMA > state->memory[TIMA_REGISTER]) {
        state->memory[TIMA_REGISTER] = state->memory[TMA_REGISTER];
        // TODO: trigger timer overflow interrupt
      }
    }

    n_instrs++;
  }
}