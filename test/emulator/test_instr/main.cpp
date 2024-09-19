#include <unity.h>
#include <Arduino.h>
#include "load/loadinstr.h"
#include "arithmetic/arithmeticinstr.h"
#include "bitarithmetic/bitarithmetic.h"
#include "misc/misc.h"
#include "flowctrl/flowctrl.h"


void setup ()
{
  UNITY_BEGIN();
  runAllLoadInstrTests();
  runAllArithmeticInstrTests();
  runAllBitArithmeticInstrTests();
  runAllMiscInstrTests();
  runAllFlowCtrlInstrTests();
  UNITY_END();
}

void loop ()
{
  delay(100);
}