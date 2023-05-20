#include <Arduino.h>
#include <vs1053.hpp>
#include <ESP8266WiFi.h>
//#include <ESP8266Audio.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <mp3_example.h>
#include <mp3_example2.h>

//Enter WiFi setup here:
const char *SSID = "UPC230D88";
const char *PASSWORD = "WtznsWw8ffve";
const char *STREAM = "";

WiFiClient client;
//AudioFileSourceHTTPStream *stream;
AudioGeneratorMP3 *audio;
AudioOutputI2S *audioOutput;

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


  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);

  WiFi.begin(SSID, PASSWORD);

  //Try forever
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("..connecting to WiFi");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  MP3.playback(mp3_data, sizeof(mp3_data));
  delay(3000);
  MP3.playback(mp3_data2, sizeof(mp3_data2));
  delay(3000);
}