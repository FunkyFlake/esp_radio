#include <Arduino.h>
#include <vs1053.hpp>
#include <ESP8266WiFi.h>
#include <mp3_example.h>
#include <mp3_example2.h>
#include <ESP8266mDNS.h>

//Enter WiFi setup here:
const char *SSID = "UPC230D88";
const char *PASSWORD = "WtznsWw8ffve";

WiFiClient client;

constexpr uint8_t XCS_PIN = D1;
constexpr uint8_t XDCS_PIN = D0;
constexpr uint8_t DREQ_PIN = D2;

VS1053 MP3(XCS_PIN, XDCS_PIN, DREQ_PIN);   

uint8_t mp3Buffer[32]; 

void setup() {
  Serial.begin(9600, SERIAL_8N1);
  delay(2000);

  uint16_t samplerate = 44100;
  MP3.init(samplerate);
  MP3.set_volume(80);

  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while(WiFi.status() != WL_CONNECTED){
    Serial.println("..connecting to WiFi");
    delay(500);
  }
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  // connect to url: http://s4-webradio.rockantenne.de/rockantenne
  int status = 0;
  while(status != 1) {
    status = client.connect("s3-webradio.rockantenne.de", 80);
    //status = client.connect("radiostreaming.ert.gr", 80);
  }
  Serial.println("Connected to radio station!");
  // Request stream
  client.print("GET /rockantenne/stream/mp3 HTTP/1.1\r\nHost: s3-webradio.rockantenne.de\r\nConnection: close\r\n\r\n");
  //client.print("GET /ert-kosmos HTTP/1.1\r\nHost: radiostreaming.ert.gr\r\nConnection: close\r\n\r\n");   
}

void loop() {
  if (client.available() > 0)
  {
    uint8_t bytesread = client.read(mp3Buffer, 32);
    MP3.playback(mp3Buffer, bytesread);
  }

  //MP3.playback(mp3_data, sizeof(mp3_data));
  //delay(3000);
  //MP3.playback(mp3_data2, sizeof(mp3_data2));
  //delay(3000);
}