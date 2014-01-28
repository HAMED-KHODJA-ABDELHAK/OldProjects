/*
 * Lab 1 Part 6 - Read the distance from an IR sensor..
 */
#include "TimerOne.h"
 
const int LED_PIN = 25;  
const int READ_PIN = 38;
int val = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(READ_PIN, INPUT); // Input for analog.
  Serial.begin(9600);
}

void loop() {
  val = analogRead(READ_PIN);
  Serial.print("The distance is: ");
  Serial.println(val);
  delay(500);   
}

