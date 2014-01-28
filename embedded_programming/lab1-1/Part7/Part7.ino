/*
 * Lab Part 7 - Read the distance from the IR sensor based on interrupt timer.
 */
#include "TimerThree.h"
 
const int LED_PIN = 25;  
const int READ_PIN = 38;
volatile int val = 0;

void readSensor(void) 
{
  val = analogRead(READ_PIN);
}

void setup() {
  pinMode(READ_PIN, INPUT);
  Serial.begin(9600);
  Timer3.initialize(500000);
  Timer3.attachInterrupt(readSensor, 500000);
  Timer3.start();
}

void loop() {
  Serial.print("val = ");
  Serial.println(val);
  delay(1000);   
}

