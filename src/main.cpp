#include <Arduino.h>
#include <random>

void setup() {
  Serial.begin(9600);
}

int adder (int a, int b)
{
  return a+b;
}

int sub (int a, int b)
{
  return a-b;
}

int dothing ()
{
  auto a = random();
  auto b = random();
  if (a > b) {
    return adder(a,b);
  } else {
    return sub(a,b);
  }
}

void loop() {
  auto t_ini = micros();
  for (int i = 0; i < int(1e6); i++) {
    int res = dothing();
  }
  auto t_fi = micros();
  Serial.println("Took " +  String((t_fi-t_ini)/1e6) + " secs.");
  delay(1);
}