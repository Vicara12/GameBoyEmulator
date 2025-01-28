#include <unity.h>
#include <Arduino.h>
#include "graphicstests.h"



void setup ()
{
  UNITY_BEGIN();
  RUN_TEST(testGetMode);
  UNITY_END();
}

void loop ()
{
  delay(100);
}