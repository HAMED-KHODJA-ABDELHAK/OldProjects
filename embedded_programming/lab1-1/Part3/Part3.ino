/*
 * Lab 1 Part 3  - LED will cycle through 4 brightness states.
 */
const int LED_PIN = 25;  
const int READ_PIN = 38;
int val = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(READ_PIN, INPUT_PULLUP);
}

void loop() {
  analogWrite(LED_PIN, val);
 
  val += 64;
  if (val == 256) 
  {
    val = 0;
  }
  
  delay(500);
}

