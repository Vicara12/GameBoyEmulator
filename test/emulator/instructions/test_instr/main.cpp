#include <unity.h>
#include <Arduino.h>
#include "load/loadinstr.h"
#include "arithmetic/arithmeticinstr.h"
#include "bitarithmetic/bitarithmetic.h"
#include "misc/misc.h"


void setup ()
{
  UNITY_BEGIN();
  runAllLoadInstrTests();
  runAllArithmeticInstrTests();
  runAllBitArithmeticInstrTests();
  runAllMiscInstrTests();
  UNITY_END();
}

void loop ()
{
  delay(100);
}