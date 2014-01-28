/*
 * Lab 1 Part 4 - Cycle the brightness through 4 LED states and print value to serial monitor.
 */
 
const int LED_PIN = 25;  
const int READ_PIN = 38;
int val = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(READ_PIN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  analogWrite(LED_PIN, val);
  val += 64;
  if (val == 256) 
  {
    val = 0;
  }
  
  Serial.print("The value is: ");
  Serial.println(val);
  
  delay(500);
}

