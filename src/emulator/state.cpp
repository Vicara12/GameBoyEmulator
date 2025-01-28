#include "emulator/state.h"

ulong getDivFromTAC (Byte value_TAC)
{
  switch (GET_TAC_CLOCK_SEL(value_TAC))
  {
  case 0b00:
    return (1 << 10);
  case 0b01:
    return (1 << 4);
  case 0b10:
    return (1 << 6);
  case 0b11:
    return (1 << 8);
  default:
    break;
  }
  return 1;
}