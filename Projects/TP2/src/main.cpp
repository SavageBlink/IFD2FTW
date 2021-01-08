#include <Arduino.h>

int analogPin = A0;
int tension = 0;
int lux = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(analogPin, INPUT);
}

void loop()
{
  tension= analogRead(analogPin);
  lux=map(tension,0,1023,10,10000);
  Serial.println(lux);
}