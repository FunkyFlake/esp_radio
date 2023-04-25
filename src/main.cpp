#include <Arduino.h>
#include <vs1053.hpp>
#include <mp3_example.h>
#include <mp3_example2.h>

constexpr uint8_t XCS_PIN = D1;
constexpr uint8_t XDCS_PIN = D0;
constexpr uint8_t DREQ_PIN = D2;

VS1053 MP3(XCS_PIN, XDCS_PIN, DREQ_PIN);   

void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600, SERIAL_8N1);
  delay(2000);

  uint16_t samplerate = 44100;
  MP3.init(samplerate);

  MP3.set_volume(80);
}


void loop() {
  MP3.playback(mp3_data, sizeof(mp3_data));
  delay(3000);
  MP3.playback(mp3_data2, sizeof(mp3_data2));
  delay(3000);
}