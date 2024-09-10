#include <Arduino.h>
#include "instructions/instruction.h"

void setup() {
  Serial.begin(9600);
}

void loop() {
  State state;
  int total = 0;
  auto t_ini = micros();
  for (int i = 0; i < int(1e6); i++) {
    total += executeInstruction(0x06, 0x01, state);
  }
  auto t_fi = micros();
  Serial.println("Took " +  String((t_fi-t_ini)/1e6) + " secs " + String(total) + ".");
  delay(1);
}