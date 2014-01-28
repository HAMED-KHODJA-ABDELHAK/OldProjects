/*
 * Lab 1 Part 2 - Manipulate an LED based on a digital read value. 
 */
 
const int LED_PIN = 25;  
const int READ_PIN = 38;

// the setup() method runs once, when the sketch starts
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(READ_PIN, INPUT_PULLUP);
}

/* If the READ_PIN is high LED will blink fast.
   Else LED will blink slowly. */
void loop() {
  if (digitalRead(READ_PIN)) 
  {
    digitalWrite(LED_PIN, LOW);
    delay(400);
    digitalWrite(LED_PIN, HIGH);
    delay(400);
  } 
  else 
  {
    digitalWrite(LED_PIN, LOW);
    delay(2000);
    digitalWrite(LED_PIN, HIGH);
    delay(2000);
  } 
}

