#include <unity.h>
#include <Arduino.h>
#include "load/loadinstr.h"
#include "arithmetic/arithmeticinstr.h"


void setup ()
{
  UNITY_BEGIN();
  runAllLoadInstrTests();
  runAllArithmeticInstrTests();
  UNITY_END();
}

void loop ()
{
  delay(100);
}