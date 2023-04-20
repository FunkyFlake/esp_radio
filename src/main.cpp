#include <Arduino.h>
#include <vs1053.hpp>

void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600, SERIAL_8N1);
  constexpr uint8_t XCS_PIN = D1;
  constexpr uint8_t XDCS_PIN = D0;
  constexpr uint8_t DREQ_PIN = D2;
  delay(5000);

  VS1053 MP3 = VS1053(XCS_PIN, XDCS_PIN, DREQ_PIN);   
  MP3.init();
}


void loop() {
  // Write a message to the serial monitor
  Serial.println("Hello, world!\n");

  // Wait for a moment
  delay(1000);
}