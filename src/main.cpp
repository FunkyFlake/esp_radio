#include <Arduino.h>

void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);
}

void loop() {
  // Write a message to the serial monitor
  Serial.println("Hello, world!\n");

  // Wait for a moment
  delay(1000);
}