/*
 * Lab Part 5 - Make the LED phase blink based on a timer while main loop continues uninterrupted.
 */
#include "TimerOne.h"
 
const int LED_PIN = 25;  

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Timer1.initialize(500000);
  Timer1.pwm(LED_PIN, 512);
  Timer1.start();
}

void loop() {
  Serial.println("Testing.");
  delay(2000);   
}

