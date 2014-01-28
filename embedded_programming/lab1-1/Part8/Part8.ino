/*
 * Lab Part 8 - Input received from serial monitor manipulates the period of the interupt handler.
 */
#include "TimerThree.h"
 
const int LED_PIN = 25;  
const int READ_PIN = 38; 
volatile int val = 0;

void readSensor(void) 
{
  val = analogRead(READ_PIN);
}

void initReadSensor(int period) 
{
  Timer3.stop();
  Timer3.attachInterrupt(readSensor, period);
  Timer3.start();
}

void setup() {
  pinMode(READ_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  initReadSensor(500000);
}

void loop() {
  char c = '\0', cmd = '\0';
  unsigned long l = 0;
  
  if (Serial.available()) 
  {
    cmd = Serial.read();
    
    while (Serial.available()) 
    {   
      c = Serial.read();
      l *= 10; 
      l += c - '0';
    }   
    Serial.print("cmd = ");
    Serial.println(cmd);
    Serial.print("l = ");
    Serial.println(l);
  
    if (cmd == 'b')
    {
		// Do something.
    }
    else if (cmd == 'i')
    {
      // Investigate how period manipulated.
      initReadSensor(l);
    }    
  }
 
  if (val != 0)
  { 
    Serial.print("val = ");
    Serial.println(val);
  }
  else 
  {
	Serial.println("Data not ready.");
  }

  delay(50);  
}




