#include <Arduino.h>


int led = 7;

void setup()
{
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  analogWrite(led,HIGH);
  delay(500);
  analogWrite(led,LOW);
  delay(500);
}