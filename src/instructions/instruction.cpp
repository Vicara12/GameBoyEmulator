#include "instructions/instruction.h"
#include "instructions/loadinstr.h"

int executeInstruction (Byte opcode, Byte data, State& state)
{
  switch (opcode)
  {
  case 0x00:
    return -1;
  case 0x01:
    return -1;
  case 0x02:
    return -1;
  case 0x03:
    return -1;
  case 0x04:
    return -1;
  case 0x05:
    return -1;
  case 0x06:
    return instr_LD_nn_n(state.B, data);
  case 0x07:
    return -1;
  case 0x08:
    return -1;
  case 0x09:
    return -1;
  case 0x0A:
    return -1;
  case 0x0B:
    return -1;
  case 0x0C:
    return -1;
  case 0x0D:
    return -1;
  case 0x0E:
    return instr_LD_nn_n(state.C, data);
  case 0x0F:
    return -1;
  case 0x10:
    return -1;
  case 0x11:
    return -1;
  case 0x12:
    return -1;
  case 0x13:
    return -1;
  case 0x14:
    return -1;
  case 0x15:
    return -1;
  case 0x16:
    return instr_LD_nn_n(state.D, data);
  case 0x17:
    return -1;
  case 0x18:
    return -1;
  case 0x19:
    return -1;
  case 0x1A:
    return -1;
  case 0x1B:
    return -1;
  case 0x1C:
    return -1;
  case 0x1D:
    return -1;
  case 0x1E:
    return instr_LD_nn_n(state.E, data);
  case 0x1F:
    return -1;
  case 0x20:
    return -1;
  case 0x21:
    return -1;
  case 0x22:
    return -1;
  case 0x23:
    return -1;
  case 0x24:
    return -1;
  case 0x25:
    return -1;
  case 0x26:
    return instr_LD_nn_n(state.H, data);
  case 0x27:
    return -1;
  case 0x28:
    return -1;
  case 0x29:
    return -1;
  case 0x2A:
    return -1;
  case 0x2B:
    return -1;
  case 0x2C:
    return -1;
  case 0x2D:
    return -1;
  case 0x2E:
    return instr_LD_nn_n(state.L, data);
  case 0x2F:
    return -1;
  case 0x30:
    return -1;
  case 0x31:
    return -1;
  case 0x32:
    return -1;
  case 0x33:
    return -1;
  case 0x34:
    return -1;
  case 0x35:
    return -1;
  case 0x36:
    return instr_LD_mem_r1_nn(REG_HL(state), data, state, true);
  case 0x37:
    return -1;
  case 0x38:
    return -1;
  case 0x39:
    return -1;
  case 0x3A:
    return -1;
  case 0x3B:
    return -1;
  case 0x3C:
    return -1;
  case 0x3D:
    return -1;
  case 0x3E:
    return -1;
  case 0x3F:
    return -1;
  case 0x40:
    return instr_LD_r1_r2(state.B, state.B);
  case 0x41:
    return instr_LD_r1_r2(state.B, state.C);
  case 0x42:
    return instr_LD_r1_r2(state.B, state.D);
  case 0x43:
    return instr_LD_r1_r2(state.B, state.E);
  case 0x44:
    return instr_LD_r1_r2(state.B, state.H);
  case 0x45:
    return instr_LD_r1_r2(state.B, state.L);
  case 0x46:
    return instr_LD_r1_mem_r2(state.B, REG_HL(state), state);
  case 0x47:
    return -1;
  case 0x48:
    return instr_LD_r1_r2(state.C, state.B);
  case 0x49:
    return instr_LD_r1_r2(state.C, state.C);
  case 0x4A:
    return instr_LD_r1_r2(state.C, state.D);
  case 0x4B:
    return instr_LD_r1_r2(state.C, state.E);
  case 0x4C:
    return instr_LD_r1_r2(state.C, state.H);
  case 0x4D:
    return instr_LD_r1_r2(state.C, state.L);
  case 0x4E:
    return instr_LD_r1_mem_r2(state.C, REG_HL(state), state);
  case 0x4F:
    return -1;
  case 0x50:
    return instr_LD_r1_r2(state.D, state.B);
  case 0x51:
    return instr_LD_r1_r2(state.D, state.C);
  case 0x52:
    return instr_LD_r1_r2(state.D, state.D);
  case 0x53:
    return instr_LD_r1_r2(state.D, state.E);
  case 0x54:
    return instr_LD_r1_r2(state.D, state.H);
  case 0x55:
    return instr_LD_r1_r2(state.D, state.L);
  case 0x56:
    return instr_LD_r1_mem_r2(state.D, REG_HL(state), state);
  case 0x57:
    return -1;
  case 0x58:
    return instr_LD_r1_r2(state.E, state.B);
  case 0x59:
    return instr_LD_r1_r2(state.E, state.C);
  case 0x5A:
    return instr_LD_r1_r2(state.E, state.D);
  case 0x5B:
    return instr_LD_r1_r2(state.E, state.E);
  case 0x5C:
    return instr_LD_r1_r2(state.E, state.H);
  case 0x5D:
    return instr_LD_r1_r2(state.E, state.L);
  case 0x5E:
    return instr_LD_r1_mem_r2(state.E, REG_HL(state), state);
  case 0x5F:
    return -1;
  case 0x60:
    return instr_LD_r1_r2(state.H, state.B);
  case 0x61:
    return instr_LD_r1_r2(state.H, state.C);
  case 0x62:
    return instr_LD_r1_r2(state.H, state.D);
  case 0x63:
    return instr_LD_r1_r2(state.H, state.E);
  case 0x64:
    return instr_LD_r1_r2(state.H, state.H);
  case 0x65:
    return instr_LD_r1_r2(state.H, state.L);
  case 0x66:
    return instr_LD_r1_mem_r2(state.H, REG_HL(state), state);
  case 0x67:
    return -1;
  case 0x68:
    return instr_LD_r1_r2(state.L, state.B);
  case 0x69:
    return instr_LD_r1_r2(state.L, state.C);
  case 0x6A:
    return instr_LD_r1_r2(state.L, state.D);
  case 0x6B:
    return instr_LD_r1_r2(state.L, state.E);
  case 0x6C:
    return instr_LD_r1_r2(state.L, state.H);
  case 0x6D:
    return instr_LD_r1_r2(state.L, state.L);
  case 0x6E:
    return instr_LD_r1_mem_r2(state.L, REG_HL(state), state);
  case 0x6F:
    return -1;
  case 0x70:
    return instr_LD_mem_r1_nn(REG_HL(state), state.B, state, false);
  case 0x71:
    return instr_LD_mem_r1_nn(REG_HL(state), state.C, state, false);
  case 0x72:
    return instr_LD_mem_r1_nn(REG_HL(state), state.D, state, false);
  case 0x73:
    return instr_LD_mem_r1_nn(REG_HL(state), state.E, state, false);
  case 0x74:
    return instr_LD_mem_r1_nn(REG_HL(state), state.H, state, false);
  case 0x75:
    return instr_LD_mem_r1_nn(REG_HL(state), state.L, state, false);
  case 0x76:
    return -1;
  case 0x77:
    return -1;
  case 0x78:
    return instr_LD_r1_r2(state.A, state.B);
  case 0x79:
    return instr_LD_r1_r2(state.A, state.C);
  case 0x7A:
    return instr_LD_r1_r2(state.A, state.D);
  case 0x7B:
    return instr_LD_r1_r2(state.A, state.E);
  case 0x7C:
    return instr_LD_r1_r2(state.A, state.H);
  case 0x7D:
    return instr_LD_r1_r2(state.A, state.L);
  case 0x7E:
    return instr_LD_r1_mem_r2(state.A, REG_HL(state), state);
  case 0x7F:
    return instr_LD_r1_r2(state.A, state.A);
  case 0x80:
    return -1;
  case 0x81:
    return -1;
  case 0x82:
    return -1;
  case 0x83:
    return -1;
  case 0x84:
    return -1;
  case 0x85:
    return -1;
  case 0x86:
    return -1;
  case 0x87:
    return -1;
  case 0x88:
    return -1;
  case 0x89:
    return -1;
  case 0x8A:
    return -1;
  case 0x8B:
    return -1;
  case 0x8C:
    return -1;
  case 0x8D:
    return -1;
  case 0x8E:
    return -1;
  case 0x8F:
    return -1;
  case 0x90:
    return -1;
  case 0x91:
    return -1;
  case 0x92:
    return -1;
  case 0x93:
    return -1;
  case 0x94:
    return -1;
  case 0x95:
    return -1;
  case 0x96:
    return -1;
  case 0x97:
    return -1;
  case 0x98:
    return -1;
  case 0x99:
    return -1;
  case 0x9A:
    return -1;
  case 0x9B:
    return -1;
  case 0x9C:
    return -1;
  case 0x9D:
    return -1;
  case 0x9E:
    return -1;
  case 0x9F:
    return -1;
  case 0xA0:
    return -1;
  case 0xA1:
    return -1;
  case 0xA2:
    return -1;
  case 0xA3:
    return -1;
  case 0xA4:
    return -1;
  case 0xA5:
    return -1;
  case 0xA6:
    return -1;
  case 0xA7:
    return -1;
  case 0xA8:
    return -1;
  case 0xA9:
    return -1;
  case 0xAA:
    return -1;
  case 0xAB:
    return -1;
  case 0xAC:
    return -1;
  case 0xAD:
    return -1;
  case 0xAE:
    return -1;
  case 0xAF:
    return -1;
  case 0xB0:
    return -1;
  case 0xB1:
    return -1;
  case 0xB2:
    return -1;
  case 0xB3:
    return -1;
  case 0xB4:
    return -1;
  case 0xB5:
    return -1;
  case 0xB6:
    return -1;
  case 0xB7:
    return -1;
  case 0xB8:
    return -1;
  case 0xB9:
    return -1;
  case 0xBA:
    return -1;
  case 0xBB:
    return -1;
  case 0xBC:
    return -1;
  case 0xBD:
    return -1;
  case 0xBE:
    return -1;
  case 0xBF:
    return -1;
  case 0xC0:
    return -1;
  case 0xC1:
    return -1;
  case 0xC2:
    return -1;
  case 0xC3:
    return -1;
  case 0xC4:
    return -1;
  case 0xC5:
    return -1;
  case 0xC6:
    return -1;
  case 0xC7:
    return -1;
  case 0xC8:
    return -1;
  case 0xC9:
    return -1;
  case 0xCA:
    return -1;
  case 0xCB:
    return -1;
  case 0xCC:
    return -1;
  case 0xCD:
    return -1;
  case 0xCE:
    return -1;
  case 0xCF:
    return -1;
  case 0xD0:
    return -1;
  case 0xD1:
    return -1;
  case 0xD2:
    return -1;
  case 0xD3:
    return -1;
  case 0xD4:
    return -1;
  case 0xD5:
    return -1;
  case 0xD6:
    return -1;
  case 0xD7:
    return -1;
  case 0xD8:
    return -1;
  case 0xD9:
    return -1;
  case 0xDA:
    return -1;
  case 0xDB:
    return -1;
  case 0xDC:
    return -1;
  case 0xDD:
    return -1;
  case 0xDE:
    return -1;
  case 0xDF:
    return -1;
  case 0xE0:
    return -1;
  case 0xE1:
    return -1;
  case 0xE2:
    return -1;
  case 0xE3:
    return -1;
  case 0xE4:
    return -1;
  case 0xE5:
    return -1;
  case 0xE6:
    return -1;
  case 0xE7:
    return -1;
  case 0xE8:
    return -1;
  case 0xE9:
    return -1;
  case 0xEA:
    return -1;
  case 0xEB:
    return -1;
  case 0xEC:
    return -1;
  case 0xED:
    return -1;
  case 0xEE:
    return -1;
  case 0xEF:
    return -1;
  case 0xF0:
    return -1;
  case 0xF1:
    return -1;
  case 0xF2:
    return -1;
  case 0xF3:
    return -1;
  case 0xF4:
    return -1;
  case 0xF5:
    return -1;
  case 0xF6:
    return -1;
  case 0xF7:
    return -1;
  case 0xF8:
    return -1;
  case 0xF9:
    return -1;
  case 0xFA:
    return -1;
  case 0xFB:
    return -1;
  case 0xFC:
    return -1;
  case 0xFD:
    return -1;
  case 0xFE:
    return -1;
  case 0xFF:
    return -1;
  default:
    return -1;
  }
}